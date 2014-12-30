/******************
 DBWORD.H/CPP
 LAST MAJOR UPDATE
 12 / 29 / 2014
 ******************/

#include "dbword.h"

// constructor
DBWord::DBWord( string word, bool sentence_terminator ) : Word( word, sentence_terminator )
{
}

DBWord::DBWord( bool is_root ) : Word( is_root )
{
}

/*void Word::printWords()
{
	for ( map<std::string, WordWithCount>::iterator iter = _words.begin(); iter != _words.end(); iter++ )
	{
		cout << (*iter).second.word->getWord() << ": " << (*iter).second.count << endl;
	}
}*/