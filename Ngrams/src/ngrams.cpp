// program reads an
//input file and uses it to build a large data
//structure of word groups called "Ngrams"
//as a basis for randomly generating
//new text that sounds like it came from
//the same author as that file

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "hashmap.h"
#include "hashset.h"
#include "map.h"
#include "queue.h"
#include "random.h"
#include "set.h"
#include "simpio.h"
#include "vector.h"

using namespace std;


void createMap(int n, Vector<string>, HashMap< Vector<string>, Vector<string> > &m);
void createNGram(int n, int nR, HashMap< Vector<string>, Vector<string> > &m);

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Courier New-16");
    setConsoleEcho(true);

    //Prompting user for parameters
    //string filename = getLine("File name?");
    string filename = "tomsawyer.txt";

    //setting up
    ifstream ifs;
    openFile(ifs, filename);
    Vector<string> text;

    //read file to vector
    while(!ifs.fail())
    {
        string token;
        ifs >> token;
        text.add(token);
    }
    ifs.close();

    HashMap< Vector<string>, Vector<string> >map;


    //creating NGram
    while(1)
    {
        int n = getInteger("Value of N? (0 to quit)");
        if (n == 0)
            break;
        int nR = getInteger("# of random words to generate (0 to quit)?");
        if (nR == 0)
            break;
        map.clear();
        createMap(n, text, map);

        createNGram(n, nR, map);
    }
    cout << "Exiting." << endl;
    return 0;
}
void createMap(int n, Vector<string> text, HashMap< Vector<string>, Vector<string> > &m)
{

    //go through vector and fill the map
    for (int k = 0; k < text.size() - n; k++)
    {
        Vector<string> key;
        //add n-1 words to key
        for (int i = 0; i < n-1; i++)
        {
            key.add(text[i+k]);
        }
        //if map has not such key, create it
        if (!m.containsKey(key))
        {
            Vector<string> v;
            m.put(key, v);
        }
        //add next word as a value to the map
        m[key].add(text[k + n - 1]);
    }
    //wrap around the start to add n-1 more pairs
    for (int k = 0; k < n - 1; k++)
    {
        Vector<string> key;
        for (int i = 0; i < n-1; i++)
        {
            int index = text.size() - n + i + k;
            int times = index/(text.size() - 1); //rounds to 0 or 1
            key.add(text[index - times*(text.size() - 1)]);
        }
        if (!m.containsKey(key))
        {
            Vector<string> v;
            m.put(key, v);
        }

        m[key].add(text[k]);
    }
}

void createNGram(int n, int nR, HashMap< Vector<string>, Vector<string> > &m)
{
    Vector<string> output;

    //randomly choose where to start from
    Vector<string> randomKey;
    //setRandomSeed(1);
    int randInt = randomInteger(0, m.keys().size() - 1);
    randomKey = m.keys()[randInt];

    //add n-1 words from random key
    output+=randomKey;

    Vector<string> value = m[randomKey];
    randInt = randomInteger(0, value.size() - 1);

    //add random word from map value of that key
    output+=value[randInt];

    //slide window for key to get nR words
    for (int i = 0; i < nR - n; i++)
    {
        Vector<string> window;
        //fill widow with n-1 words
        for (int k = 1; k < n; k++)
        {
            window.add(output[i + k]);
        }

        //add random word from map value of key=window
        value = m[window];
        randInt = randomInteger(0, value.size() - 1);
        output+=value[randInt];

    }
    string result;
    for (int i = 0; i < output.size(); i++)
    {
        result+=output[i];
        result+=" ";
    }
    cout << "..." << result << "..." << endl;
}
