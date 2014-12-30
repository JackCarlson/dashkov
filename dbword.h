/******************
 DBWORD.H/CPP
 LAST MAJOR UPDATE
 12 / 29 / 2014
 ******************/

#ifndef DBWORD_H
#define dbword_h

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <time.h>
#include <algorithm>
#include "dashkov.h"

using namespace std;

class DBWord;

class DBWord : public Word
{
public:
	// constructor
	DBWord(){}
	DBWord( string word, bool sentence_terminator );
	DBWord( bool is_root );

	// destructor
	~DBWord() {}

	/*Word * addWord ( string in_word, bool sentence_terminator );

	Word* addWord( Word* in_word );

	// used for the root node only
	Word* seed( string in_word, Word * prev_word, bool sentence_terminator );

    Word* searchContext ( string in_context );

	string generate( int max_words );

	string getWord() { return _word; }
	int getOccurrences() { return _occurrences; }
	void incOccurrences() { _occurrences++; }
	int getAvgOccurrences() { return _avg_occurrences; }
	void calcAvgOccurrences() {}
	bool isRoot() { return _is_root; }
    bool isTerminator() { return _terminator; }

	int getWordCount() { return _words.size(); }

	map<std::string, WordWithCount>::iterator getBeginIter() { return _words.begin(); }
	map<std::string, WordWithCount>::iterator getEndIter() { return _words.end(); }

    void printWords();

    static bool cmpContextVect(string lhs, string rhs) { return (int)lhs.size() > (int)rhs.size(); }

private:

    string _word;
	int _occurrences;
	int _avg_occurrences;
	bool _is_root;
	map<std::string, WordWithCount> _words;
    bool _debug;
    bool _terminator;*/
};

#endif