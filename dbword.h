/******************
 DBWORD.H/CPP
 LAST MAJOR UPDATE
 01 / 03 / 2015
 ******************/

#ifndef DBWORD_H
#define dbword_h

#include "dashkov.h"
#include <sqlite3.h>

using namespace std;

class DBWord;

class DBWord
{
public:

    // constructor / destructor
    DBWord();
    ~DBWord();

    void loadFromDb();

    Word * learn( string new_word, Word * prevWord, bool sentence_terminator );

    string generate( string context, int maxWords);

    unsigned getWordCount();

private:

    bool sqliteOpenDB();
    int getWordIdFromDB( string in_word );
    int insertWordToDB( string in_word, bool sentence_terminator ); // returns new word ID
    void updateWordOccurrenceFromWordID( int word_id );
    void linkWords( int startWordId, int nextWordId );

    Word * _rootWord;
    sqlite3 * _db;

};

#endif