//File: wordladder.cpp
//This program takes two words from user and converts
//one into another by changing one letter at a time

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
#include "stack.h"
#include "vector.h"

using namespace std;

void ReadDictionary(Set<string> &set);
void printStack(Stack<string> s);
Set<string> findNeighbors(string str);
Set<string> dict;

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Courier New-16");
    setConsoleEcho(true);

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    //read file to set of strings
    ReadDictionary(dict);

    string word1;
    string word2;

    while (1)
    {
        while (1)
        {
            cout << "Please type two words of the same length: ";
            cin >> word1 >> word2;
            cout << endl;
            if ((word1.length() == word2.length())&&(dict.contains(word1))&&(dict.contains(word2)))
                break;
        }

        //Build ladder
        Queue< Stack<string> > q;
        Stack<string> startStack;
        startStack.push(word1);
        q.enqueue(startStack);
        Set<string> usedWords;
        usedWords.add(word1);

        while (!q.isEmpty())
        {
            Stack<string> s = q.dequeue();
            string peek = s.peek();
            if (peek == word2)
            {
                cout << "Ladder from " << word2 << " back to " << word1 << ":" << endl;
                printStack(s);
                cout << endl;
                break;
            }
            else
            {
                Set<string> neighbors = findNeighbors(peek);
                neighbors -= usedWords;

                foreach(string word in neighbors)
                {
                    Stack<string> sCopy;
                    sCopy = s;
                    sCopy.push(word);
                    q.enqueue(sCopy);
                    usedWords+=neighbors;
                }

            }
        }
        cout << "repeat? (y/n) ";
        string answer;
        cin >> answer;
        if(answer == "n")
            break;
    }


    cout << "Have a nice day." << endl;
    return 0;
}

void printStack(Stack<string> s)
{
    while (!s.isEmpty())
        cout << s.pop() << " ";
}
Set<string> findNeighbors(string nextWord)
{
    string change = nextWord;
    Set<string> result;
    int len = change.size();

    for(int i = 0; i < len; i++)
    {
        for(int k = 1; k < 27; k++)
        {
            int times = ((int)change[i] + 1 - 'a')/26;
            change[i] = change[i] + 1 - times*26;
            if (dict.contains(change))
                result.add(change);
        }
    }

    return result;
}
//Reads file "dictionary.txt" and writes every line to the set
void ReadDictionary(Set<string>& set)
{
    ifstream file;
    openFile(file, "dictionary.txt");
    string line;
    while (getline(file, line))
    {
        set.add(line);
    }
    file.close();
}
