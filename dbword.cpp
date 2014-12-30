/******************
 DBWORD.H/CPP
 LAST MAJOR UPDATE
 12 / 29 / 2014
 ******************/

#include "dbword.h"

// constructor
DBWord::DBWord( string word, bool sentence_terminator )
{
	_word = word;
	_occurrences = 1;
	_avg_occurrences = 1;
    _debug = false;
    _terminator = sentence_terminator;
}

DBWord::DBWord( bool is_root )
{
	_word = "";
	_occurrences = 1;
	_avg_occurrences = 1;
	_is_root = is_root;
    _debug = false;
}

/*void Word::printWords()
{
	for ( map<std::string, WordWithCount>::iterator iter = _words.begin(); iter != _words.end(); iter++ )
	{
		cout << (*iter).second.word->getWord() << ": " << (*iter).second.count << endl;
	}
}*/