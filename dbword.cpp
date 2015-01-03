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

    if (sqlite3_prepare( _db, sql.c_str(), -1, &statement, 0 ) == SQLITE_OK)
    {
        int res = 0;
        res = sqlite3_step( statement );

        if ( res == SQLITE_DONE ) // not able to find the tables, create them
        {
            // add the WORD table
            sql = "CREATE TABLE WORD( " \
            "ID		INT PRIMARY KEY	NOT NULL, " \
            "WORD		TEXT		NOT NULL, " \
            "TERMINATOR	INT		NOT NULL);";

            result = sqlite3_exec(_db, sql.c_str(), 0, &statement, 0);
            if ( result != SQLITE_OK )
            {
                cout << "Unable to add WORD table!  Failing..." << endl;
                exit( 0 );
            }

            // add the WORD_MAP table
            sql = "CREATE TABLE WORD_MAP( " \
            "ID_KEYWORD	INT	NOT NULL, " \
            "ID_LINKED	INT	NOT NULL);";

            result = sqlite3_exec(_db, sql.c_str(), 0, &statement, 0);
            if ( result != SQLITE_OK )
            {
                cout << "Unable to add WORD_MAP table!  Failing..." << endl;
                exit( 0 );
            }

        }
    }

    return true;
}

Word * DBWord::learn( string new_word, Word * prevWord, bool sentence_terminator )
{
    return _rootWord->seed( new_word, prevWord, sentence_terminator );
}

string DBWord::generate( string context, int maxWords)
{
    return _rootWord->searchContext(context)->generate(maxWords);
}

unsigned DBWord::getWordCount()
{
    return _rootWord->getWordCount();
}
