/******************
 DBWORD.H/CPP
 LAST MAJOR UPDATE
 01 / 03 / 2015
 ******************/

#include "dbword.h"

// constructor
DBWord::DBWord()
{
    _rootWord = new Word(true);

    sqliteOpenDB();
}

DBWord::~DBWord()
{
    sqlite3_close( _db );
}

bool DBWord::sqliteOpenDB()
{
    char *zErrMsg = 0;
    int result;

    result = sqlite3_open( "dashkov.db", &_db );

    if ( result != SQLITE_OK ) // if connection failed to open
    {
        cout << "ERROR: unable to open DB!  Failing... " << endl << sqlite3_errmsg( _db ) << endl;
        exit( 0 );
    }

    ////////////////////////////////////////
    // check if the required tables exist //
    //        if not, create them         //
    ////////////////////////////////////////

    string sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='WORD';";
    sqlite3_stmt *statement;

    if ( sqlite3_prepare_v2( _db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {

        int res = 0;
        res = sqlite3_step( statement );

        if ( res == SQLITE_DONE ) // not able to find the tables, create them
        {
            // add the WORD table
            sql = "CREATE TABLE WORD( " \
            "WORD		TEXT	NOT NULL, " \
            "OCCURRENCES INT     NOT NULL, " \
            "TERMINATOR	INT		NOT NULL);";

            result = sqlite3_exec( _db, sql.c_str(), 0, &statement, 0 );
            if ( result != SQLITE_OK )
            {
                cout << "Unable to add WORD table!  Failing..." << endl;
                exit( 0 );
            }

            // add the WORD_MAP table
            sql = "CREATE TABLE WORD_MAP( " \
            "WORD_ID	INT	NOT NULL, " \
            "NEXT_ID	INT	NOT NULL, " \
            "OCCURRENCES	INT	NOT NULL);";

            result = sqlite3_exec( _db, sql.c_str(), 0, &statement, 0 );
            if ( result != SQLITE_OK )
            {
                cout << "Unable to add WORD_MAP table!  Failing..." << endl;
                exit( 0 );
            }

            // add the WORD_MAP index
            sql = "CREATE UNIQUE INDEX word_map_idx ON word_map(word_id, next_id);";

            result = sqlite3_exec( _db, sql.c_str(), 0, &statement, 0 );
            if ( result != SQLITE_OK )
            {
                cout << "Unable to add WORD_MAP index!  Failing..." << endl;
                exit( 0 );
            }
        }
    }

    sqlite3_finalize( statement );

    return true;
}

Word * DBWord::learn( string new_word, Word * prevWord, bool sentence_terminator )
{
    Word * retWord = _rootWord->seed( new_word, prevWord, sentence_terminator );

    // DB: get ID if added word exists, if not add it and get ID
    // also get ID of prevWord if not null
    // update occurence of added word
    // if prevWord not null, get if words are linked in WORD_MAP
    // if not, connect them

    int wordDbID = getWordIdFromDB( new_word );

    if ( wordDbID == -1 )
        wordDbID = insertWordToDB( new_word, sentence_terminator );
    else
        updateWordOccurrenceFromWordID( wordDbID );

    if ( prevWord != NULL )
    {
        int prevWordDbID = getWordIdFromDB( prevWord->getWord() );
        linkWords( prevWordDbID, wordDbID );
    }

    return retWord;
}

int DBWord::getWordIdFromDB( string in_word )
{
    int word_id = -1;
    string sql = "SELECT rowid FROM word WHERE word = ?;";
    sqlite3_stmt *statement;

    if ( sqlite3_prepare_v2( _db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        if ( sqlite3_bind_text( statement, 1, in_word.c_str(),
                in_word.length(), SQLITE_STATIC ) != SQLITE_OK )
        {
            // error
            cout << "getWordIdFromDB: Unable to bind word " << in_word << endl;
        }

        int num_cols = sqlite3_column_count( statement );
        int res = sqlite3_step( statement );

        if ( res == SQLITE_ROW )
        {
            string s = (char*)sqlite3_column_text( statement, 0 );
            //cout << "WORD ID: " << stoi(s) << endl;
            word_id = stoi(s);
        }
    }

    sqlite3_finalize( statement );

    return word_id;
}

int DBWord::insertWordToDB( string in_word, bool sentence_terminator )
{
    char charterm = '0';
    if ( sentence_terminator )
        charterm = '1';

    int result = 0;
    string sql = "INSERT INTO word VALUES ( ?, 1, ? );";
    sqlite3_stmt *statement;

    if ( sqlite3_prepare_v2( _db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {

        if ( sqlite3_bind_text( statement, 1, in_word.c_str(),
                               in_word.length(), SQLITE_STATIC ) != SQLITE_OK )
        {
            // error
            cout << "insertWordToDB: Unable to bind word " << in_word << endl;
        }

        if ( sqlite3_bind_int( statement, 2, (int)sentence_terminator ) != SQLITE_OK )
        {
            // error
            cout << "insertWordToDB: Unable to bind bool " << sentence_terminator << endl;
        }

        result = sqlite3_step( statement );
        if ( result != SQLITE_DONE )
        {
            cout << "Unable to add new word to WORD table!  Failing... "
                << in_word << " " << result << "" << sqlite3_errmsg(_db) << endl;
            exit( 0 );
        }
    }

    sqlite3_finalize( statement );

    return sqlite3_last_insert_rowid(_db);
}

void DBWord::updateWordOccurrenceFromWordID( int word_id )
{
    int result = 0;
    string sql = "UPDATE word SET occurrences = occurrences + 1 WHERE rowid = ?";
    sqlite3_stmt *statement;

    if ( sqlite3_prepare_v2( _db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        if ( sqlite3_bind_int( statement, 1, word_id ) != SQLITE_OK )
        {
            // error
            cout << "updateWordOccurrenceFromWordID: Unable to bind word_id "
                    << word_id << endl;
        }

        result = sqlite3_step( statement );
        if ( result != SQLITE_DONE )
        {
            cout << "Unable to occurrences in WORD table!  Failing... "
            << word_id << " " << result << "" << sqlite3_errmsg(_db) << endl;
            exit( 0 );
        }
    }

    sqlite3_finalize( statement );
}


void DBWord::linkWords( int startWordId, int nextWordId )
{
    int result = 0;
    string sql = "INSERT OR REPLACE INTO word_map (word_id, next_id, occurrences) " \
        "VALUES ( ?, ?, " \
        "COALESCE( ( SELECT occurrences FROM word_map WHERE word_id = ? AND next_id = ? ) + 1, 1 ) );";
    sqlite3_stmt *statement;

    if ( sqlite3_prepare_v2( _db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK )
    {
        if ( sqlite3_bind_int( statement, 1, startWordId ) != SQLITE_OK )
        {
            // error
            cout << "linkWords: Unable to bind startWordId #1 "
            << startWordId << endl;
        }
        if ( sqlite3_bind_int( statement, 2, nextWordId ) != SQLITE_OK )
        {
            // error
            cout << "linkWords: Unable to bind nextWordId #1 "
            << nextWordId << endl;
        }
        if ( sqlite3_bind_int( statement, 3, startWordId ) != SQLITE_OK )
        {
            // error
            cout << "linkWords: Unable to bind startWordId #2 "
            << startWordId << endl;
        }
        if ( sqlite3_bind_int( statement, 4, nextWordId ) != SQLITE_OK )
        {
            // error
            cout << "linkWords: Unable to bind nextWordId #2 "
            << nextWordId << endl;
        }

        result = sqlite3_step( statement );
        if ( result != SQLITE_DONE )
        {
            cout << "Unable to create link in WORD_MAP table!  Failing... "
            << result << "" << sqlite3_errmsg(_db) << endl;
            exit( 0 );
        }
    }

    sqlite3_finalize( statement );
}

void DBWord::loadFromDb()
{

}

/////// pass-through methods
string DBWord::generate( string context, int maxWords )
{
    return _rootWord->searchContext(context)->generate(maxWords);
}

unsigned DBWord::getWordCount()
{
    return _rootWord->getWordCount();
}

