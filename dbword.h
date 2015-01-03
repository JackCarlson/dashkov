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

    Word * learn( string new_word, Word * prevWord, bool sentence_terminator );

    string generate( string context, int maxWords);

    unsigned getWordCount();

private:

    bool sqliteOpenDB();
    static int callback( void *NotUsed, int argc, char **argv, char **azColName );

    Word * _rootWord;
    sqlite3 * _db;

};

#endif