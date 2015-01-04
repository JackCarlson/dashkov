/******************
 DASHKOV.H/CPP
 LAST MAJOR UPDATE
 01 / 03 / 2015
 ******************/

#ifndef DASHKOV_H
#define dashkov_h

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <time.h>
#include <algorithm>

using namespace std;

class Word;
struct WordWithCount;

struct WordWithCount
{
	WordWithCount() { count = 0; }
	int count;
	Word* word;
};

class Word
{
public:

    // constructor
	Word(){}
	Word( string word, bool sentence_terminator );
	Word( bool is_root );

	// destructor
	~Word() {}

	// used for the root node only
	Word * seed( string in_word, Word * prev_word, bool sentence_terminator );

    Word * searchContext ( string in_context );

	string generate( int max_words );

	string getWord() { return _word; }
	int getOccurrences() { return _occurrences; }
	void incOccurrences() { _occurrences++; }
	int getAvgOccurrences() { return _avg_occurrences; }
	void calcAvgOccurrences() {}
	bool isRoot() { return _is_root; }
    bool getTerminator() { return _terminator; }
    void setTerminator( bool terminator ) { _terminator = terminator; }

	unsigned long getWordCount() { return _words.size(); }

	map<std::string, WordWithCount>::iterator getBeginIter() { return _words.begin(); }
	map<std::string, WordWithCount>::iterator getEndIter() { return _words.end(); }

    void printWords();

    static bool cmpContextVect(string lhs, string rhs) { return (int)lhs.size() > (int)rhs.size(); }

    Word * addWord ( string in_word, bool sentence_terminator );
    Word * addWord( Word* in_word );
    void linkWords( string word_start, string word_next );

private:

    string _word;
	int _occurrences;
	int _avg_occurrences;
	bool _is_root;
	map<std::string, WordWithCount> _words;
    bool _debug;
    bool _terminator;
};

#endif