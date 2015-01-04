#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <time.h>
#include <sys/stat.h>
#include "dbword.h"

using namespace std;

DBWord * rootWord;

bool isTerminatorChar( char input )
{
    switch ( input )
    {
        case '\n':
        case '.':
        case '!':
        case '?':
        case '\'':
        case '\"':
        case '\0':
            return true;
        default:
            return false;
    }
}

void learn( const char *input )
{
    Word * prevWord = NULL;
    string str_in = input;
    string buffer = "";
    bool sentence_terminator = false;

    for ( int i = 0; i < str_in.length(); i++ )
    {
        buffer += str_in.at(i);

        if ( str_in.at(i) == '\t' || str_in.at(i) == '\n' || str_in.at(i) == ' ' || str_in.at(i) == '\0' )
        {
            if ( buffer.length() > 0 )
            {
                if ( isTerminatorChar( str_in.at(i) ) )
                    sentence_terminator = true;

                prevWord = rootWord->learn( buffer, prevWord, sentence_terminator );
            }

            // check if end of line
            if ( sentence_terminator )
            {
                prevWord = NULL;
                sentence_terminator = false; // reset bool
            }

            buffer = "";
        }
    }
}

void populateChain( const char *filename )
{
    rootWord = new DBWord();
    Word * prevWord = NULL;

    // open text file
    ifstream fin;
    fin.open( filename );
    
    bool sentence_terminator = false;

    unsigned wordCount = 0;
    string newWord = "";
    
    while( !fin.eof() )
    {
        //if (wordCount % 1000 == 0) cout << "AT: " << wordCount << endl; // debugging fun
        
        wordCount++;
        
        fin >> newWord; // read the next word

        char lastChar = newWord.at( newWord.length() - 1 );

        if ( /* fin.peek() == '\n' && */ isTerminatorChar( lastChar ) )
            sentence_terminator = true;

        prevWord = rootWord->learn( newWord, prevWord, sentence_terminator );

        // check if end of line
        if ( sentence_terminator )
        {
            prevWord = NULL;
            sentence_terminator = false; // reset bool
        }
    }

    cout << rootWord->getWordCount() << " words total." << endl;
}


const char* getResponse( const char *input, int maxwords )
{
    string result = rootWord->generate( input, maxwords );
    return result.c_str();
}

void loadFromDb()
{
    rootWord = new DBWord();
    rootWord->loadFromDb();
    cout << rootWord->getWordCount() << " DB words total." << endl;
}

inline bool fileExists ( const string& name )
{
    struct stat buffer;
    return ( stat( name.c_str(), &buffer ) == 0 );
}

int main()
{
    if ( fileExists( "dashkov.db" ) )
        loadFromDb();
    else
        populateChain( "nightmareabbey.txt" );

    for (int i = 0; i < 5; i++)
    {
        string output = getResponse( "eccentric colour persists nightly", 20 );
        cout << output << endl << endl;
    }

    return 0;
}
