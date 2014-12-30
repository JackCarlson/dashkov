#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <time.h>
#include "dbword.h"

using namespace std; 

DBWord rootWord(true);

void learn( const char *input )
{
    string str_in = input;
    string buffer = "";

    for ( int i = 0; i < str_in.length(); i++ )
    {
        buffer += str_in.at(i);

        if ( str_in.at(i) == '\t' || str_in.at(i) == '\n' || str_in.at(i) == ' ' || str_in.at(i) == '\0' )
        {
            if ( buffer.length() > 0 )
            {
                if ( str_in.at(i) == '\n' || str_in.at(i) == '.' )
                    rootWord.addWord( buffer, true );
                else
                    rootWord.addWord( buffer, false );
            }

            buffer = "";
            continue;
        }
    }
}

void populateChain(const char *filename)
{
    DBWord * prevWord = NULL;

    // open text file
    ifstream fin;
    fin.open(filename);
    
    bool sentence_terminator = false;

    unsigned wordCount = 0;
    string newWord = "";
    
    while( !fin.eof() )
    {
        //if (wordCount % 1000 == 0) cout << "AT: " << wordCount << endl; // debugging fun
        
        wordCount++;
        
        fin >> newWord; // read the next word

        char lastChar = newWord.at(newWord.length() - 1);

        if(fin.peek() == '\n' || lastChar == '.' || lastChar == '!' || lastChar == '?' || lastChar == '\'' || lastChar == '"')
            sentence_terminator = true;
        
        prevWord = (DBWord *)rootWord.seed( newWord, prevWord, sentence_terminator );

        // check if end of line
        if (sentence_terminator)
        {
            prevWord = NULL;
            sentence_terminator = false; // reset bool
        }
    }

    cout << rootWord.getWordCount() << " words total." << endl;
}

const char* getResponse(const char *input, int maxwords)
{
    string result = rootWord.searchContext(input)->generate(maxwords);
    return result.c_str();
}

int main()
{
    populateChain("nightmareabbey.txt");
    //rootWord.printWords(); // debugging fun

    for (int i = 0; i < 5; i++)
    {
        string output = getResponse("being symbiotically caged", 5 );
        //string output = rootWord.searchContext("being symbiotically caged")->generate(5);
        cout << " GNRTD SENTENCE: " << output << endl << endl;
    }

    return 0;
}
