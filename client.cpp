#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <time.h>
#include "dashkov.h"

using namespace std;

int main()
{
	Word rootWord(true);
	Word * prevWord = NULL;

	// open text file
	ifstream fin;
    fin.open("nightmareabbey.txt");
    
    bool sentence_terminator = false;

	unsigned wordCount = 0;
	string newWord = "";
	
	while( !fin.eof() )
	{
        //if (wordCount % 1000 == 0) cout << "AT: " << wordCount << endl; // debugging fun
		
		wordCount++;
		
		fin >> newWord; // read the next word
        
        if(fin.peek() == '\n' || newWord.at(newWord.length() - 1) == '.')
            sentence_terminator = true;
        
		prevWord = rootWord.seed( newWord, prevWord, sentence_terminator );

		// check if end of line
		if (sentence_terminator)
		{
			prevWord = NULL;
            sentence_terminator = false; // reset bool
		}
	}

	cout << rootWord.getWordCount() << " words total." << endl;

	//rootWord.printWords(); // debugging fun

	for (int i = 0; i < 5; i++)
	{
        cout << rootWord.searchContext("being mildew test2 somewhere test3")->generate( 15 ) << endl << endl;
	}

	return 0;
}