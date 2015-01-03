/******************
 DASHKOV.H/CPP
 LAST MAJOR UPDATE
 01 / 03 / 2015
 ******************/

#include "dashkov.h"

// constructor
Word::Word( string word, bool sentence_terminator )
{
	_word = word;
	_occurrences = 1;
	_avg_occurrences = 1;
    _debug = false;
    _terminator = sentence_terminator;
}

Word::Word( bool is_root )
{
	_word = "";
	_occurrences = 1;
	_avg_occurrences = 1;
	_is_root = is_root;
    _debug = false;
}

Word * Word::addWord ( string in_word, bool sentence_terminator )
{
	// search for the word in the map--if it exists, update the count
	map<std::string, WordWithCount>::iterator iter = _words.begin();
	iter = _words.find( in_word );
	
	if ( iter == _words.end() )
	{
		// didn't find it, add to the map
		WordWithCount wwc;
		wwc.count = 1;
		wwc.word = new Word( in_word, sentence_terminator );
		
		_words[in_word] = wwc;
	}
	else
	{
		// found it, update the count and occurences
		_words[in_word].count++;
		//_words[in_word].word->incOccurrences();
	}

	return _words[in_word].word;
}

Word* Word::addWord( Word* in_word )
{
	// search for the word in the map--if it exists, update the count
	map<std::string, WordWithCount>::iterator iter = _words.begin();
	iter = _words.find( in_word->getWord() );

	if ( iter == _words.end() )
	{
		// didn't find it, add to the map
		WordWithCount wwc;
		wwc.count = 1;
		wwc.word = in_word;

		_words[in_word->getWord()] = wwc;
	}
	else
	{
		// found it, update the count and occurences
		_words[in_word->getWord()].count++;
		//_words[in_word].word->incOccurrences();
	}

	return _words[in_word->getWord()].word;
}

// used for the root node only
Word* Word::seed( string in_word, Word * prev_word, bool sentence_terminator )
{
	if (!_is_root) return NULL;

	Word * retWord = addWord( in_word, sentence_terminator );

	if ( prev_word != NULL )
		prev_word->addWord( retWord );

	return retWord;
}

Word* Word::searchContext ( string in_context )
{
    /*
     This is probably where the most easy future improvement can come into play.
     For now, just sorts the context by largest word first (presumably most unique),
     then searches all the sub-nodes for the words in order, starts at the first found.
     if none are found, simply returns the current word
     */

    vector<string> context;
    string buffer = "";

    for ( int i = 0; i < in_context.length(); i++ )
    {
        char next = in_context.at(i);
        if ( next == '\t' || next == '\n' || next == ' ' || next == '\0' )
        {
            if (buffer.length() > 0)
                context.push_back(buffer);
            buffer = "";
            continue;
        }

        buffer += in_context.at(i);
    }

    sort( context.begin(), context.end(), cmpContextVect );

    // search for the words
    map<std::string, WordWithCount>::iterator iter;

    for ( int i = 0; i < context.size(); i++ )
    {
        //cout << "trying context : " << context.at(i) << endl;
        iter = this->getBeginIter();
        while ( iter != this->getEndIter() )
        {
            if ( iter->second.word->getWord() == context.at(i) && iter->second.word->getWordCount() > 0 )
                return iter->second.word;

            iter++;
        }
    }

    return this;
}

string Word::generate( int max_words )
{
	// init random number generator
	srand(time(NULL) + rand());
	Word* theWord = this;
	string sentence = "";

	int randnum = 0;
	int position = 0;
	map<std::string, WordWithCount>::iterator iter;

	for (int i = 0; i < max_words; i++)
	{
        if (_debug) cout << "Word count: " << theWord->getWordCount() << endl;
		if ( theWord->getWordCount() == 0 ) return sentence;

        // simple way to improve short sentences -- update later
        if ( i > ( max_words / 2 ) && theWord->isTerminator() ) return sentence;

		randnum = rand() % theWord->getWordCount();
		if (_debug) cout << " randnum: " << randnum << endl;

		position = 0;
		iter = theWord->getBeginIter();

		while ( position < randnum && iter != theWord->getEndIter() )
		{
			iter++;
			position++;
		}

		theWord = iter->second.word;
		sentence += theWord->getWord() + " ";
		if (_debug) cout << "Sentence so far: " << sentence << endl;
		
	}

	return sentence;
}

void Word::printWords()
{
	for ( map<std::string, WordWithCount>::iterator iter = _words.begin(); iter != _words.end(); iter++ )
	{
		cout << (*iter).second.word->getWord() << ": " << (*iter).second.count << endl;
	}
}