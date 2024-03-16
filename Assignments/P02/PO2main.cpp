/*****************************************************************************
*                    
*  Author:           Taris Major
*  Email:            tmajor0707@my.msutexas.edu
*  Label:            P02
*  Title:            Linear Search - Using Json and Getch
*  Course:           CMPS 3013
*  Semester:         Spring 2024
* 
*  Description:
*        This program looks through a JSON dictionary file to autocomplete a word
*        being typed in the terminal.
*        
*  Usage: 
*       - $ ./main filename
*       - This will read in a file containing whatever values to be read into our list/array. 
*       
*  Files:            
*       P02main.cpp          : driver program 
*       dictionary.json      : JSON file with words in the dictionary
*
*****************************************************************************/
#include "./headers/console.hpp"
#include "./headers/json.hpp"
#include "./headers/rang.hpp"
#include "./headers/timer.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;
using namespace rang;
using json = nlohmann::json;

    /**
     * Public : wordSearch
     * 
     * Description:
     *      Looks through the vector of matching words and filters them each time the function is called
     * 
     * Params:
     *      [vector<string>]    :  vector of strings
     *      [int]               :  number of times the function is called
     *      [string]            :  string used to filter the vector
     * 
     * Returns:
     *      Nothing
     */
void wordSearch(vector<string> &matchingWords, string& partialKey, int& iterations)
{
    vector<string> tempMWords;                          // Create an empty vector
                                                        // Loop through all the matching words
    for(int i = 0; i < matchingWords.size(); i++)
    {
        string temp = matchingWords[i];                 // Get the word
                                                        // Check each letter
        if(temp[iterations] >= partialKey[iterations])
           {
                tempMWords.push_back(temp);             // If they match, put the word in the temp vector
           }
     }   
    matchingWords = tempMWords;                         // Equate the two vectors
    iterations++;                                       // Increment the amount of times the function was 
                                                        // called
}

void errorMessage(string message) {
  cout << bgB::red << fgB::gray << message << fg::reset << bg::reset << endl;
  sleep(1);
}

int main(int argc, char **argv) {

    Timer T;                          // Create a timer
    char k;                           // Holds the character being typed
    vector<string> tempWords;         // Temp vector used in wordSearch function
    int iterations = 0;               // Holds integer value for amount of times wordSearch function is called
    string key;                       // Holds string value of matches from dictionary JSON file
    string substr = "";               // Var to concatenate letters to
    bool deleting = false;            // Bool to determine whether the deleting the last key pressed in 
                                      // the console
    
    // Load JSON object
    std::string filePath = "./data/dictionary.json";
    std::ifstream fileStream(filePath);
    json myJson;
    if (fileStream.is_open()) {
        fileStream >> myJson;
        fileStream.close();
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return 1;
    }

// While Z is not pressed in the terminal, loop through
while ((k = getch()) != 'Z') 
{
    clearConsole();

    // Tests for a backspace and if pressed deletes
    // last letter from "substr".
    if ((int)k == 127) {
      if (substr.size() > 0) {
        substr = substr.substr(0, substr.size() - 1);
        deleting = true;
        iterations--;
      }
    } else {
      deleting = false;
      // Make sure a letter was pressed and only letter
      if (!isalpha(k)) {
        errorMessage("Letters only!");
        continue;
      }

      // We know its a letter, lets make sure its lowercase.
      // Any letter with ascii value < 97 is capital so we
      // lower it.
      if ((int)k < 97) {
        k += 32;
      }
      substr += k;                  // append char to substr
    }

    if ((int)k != 32) {             // if k is not a space print it

      for (auto &c : substr)
        c = tolower(c);

        cout << substr << endl;     // Outputs to the terminal the current string being searched
 
        vector<string> matchingWords;
        T.Start();                  // Start timer to see how long it takes to load Json File
        // Iterate over all key-value pairs
        for (auto &element : myJson.items()) 
        {
            
            key = element.key();
            
            // Check if the key contains the partialKey substring
            if(key.find(substr) != string::npos)
            {
                // Found a match
                if(substr <= key)
                // Filter out all words that doesn't begin with the substring
                matchingWords.push_back(key);
            }
            
        }    
        // Filter matchingWords vector 
        for(int i = 0; i < substr.size(); i++)
        {
            wordSearch(matchingWords, substr, iterations);
        }
        T.End();

        cout << matchingWords.size() << " words found in " << (double)T.NanoSeconds() / 100000000 << " seconds" << endl;

        //If there are less than 10 matches
        if(matchingWords.size() < 10)
        {
             for(int i = 0; i < matchingWords.size(); i++)
                {
                    cout << matchingWords[i] << " ";
                }
        } 
        else
        {
            for(int i = 0; i < 10; i++)
            {
                cout << matchingWords[i] << " ";
            }
        }
    }
}
    return 0;
}