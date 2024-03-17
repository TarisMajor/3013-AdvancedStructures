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

consoleSize console_size; // used to store the size of console (width=cols and
                          // height=rows)

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

/**
* Prints a long background row of gray, with text centered.
* Not a good function since colors are hard coded, but using
* rang, I don't want to invest the time to figuere out how to
* interact with rang's enumerated class types and ostream.
* I guess you could add a switch statement to change background
* color:
* switch(color){
  "black" : cout<<bgB::black;
            break;
  "red" : cout<<bgB::red;
          break;
  ...
  ...
  ...
}
But still not a good solution.
*/
void titleBar(string title, int length = console_size.width) {
  string padding = string((length / 2) - (title.size() / 2), ' ');
  title = padding + title + padding;
  cout << bgB::black << fg::gray << title << fg::reset << bg::reset << endl;
}

/**
 * horizontalBar
 *
 * Description:
 *      Prints a horizontal bar of a given length
 * Params:
 *      int length - length of the bar
 * Returns:
 *      void
 */
void horizontalBar(int length = console_size.width) {
  string line = string(length, '_');
  cout << fg::gray << line << fg::reset << bg::reset << endl;
}

/**
 * printMatches
 *
 * Description:
 *      Prints the matches to the screen.
 * Params:
 *      vector<string> matches - vector of matches
 * Returns:
 *      void
 */
void printMenu(vector<string> options) {
  int i = 1;
  for (auto s : options) {
    cout << fg::black << style::bold << i << ". " << style::reset << fg::cyan
         << s << fg::reset << bg::reset << endl;
    i++;
  }
  cout << fg::reset << bg::reset;
}

/**
 * printHighlightedSubstr
 *
 * Description:
 *      Given a word, print the substr underlined and red vs blue for the rest
 * of the word.
 * Params:
 *      string word - word to print
 *      string substr - substring to highlight in red
 *      int loc - location of substr in word
 * Returns:
 *      void
 */
void printHighlightedSubstr(string word, string substr, int loc) {
  for (int j = 0; j < word.size(); j++) {
    // if we are printing the substring turn it red
    if (j >= loc && j <= loc + substr.size() - 1) {
      cout << fg::red << style::underline << word[j] << fg::blue
           << style::reset;
    } else {
      cout << fg::blue << word[j] << fg::reset << style::reset;
    }
  }
}

/**
 * printCurrent
 *
 * Description:
 *      Prints the current key pressed and the current substr to the screen.
 * Params:
 *      char k - last character pressed.
 *      string word - current substring being printed.
 * Returns:
 *      void
 */
void printCurrent(char k, string word) {
  cout << fg::green << style::bold << "KeyPressed: \t\t" << style::reset
       << fgB::yellow;
  if (int(k) == 127) {
    cout << "del";
  } else {
    cout << k;
  }
  cout << " = " << (int)k << fg::reset << endl;
  cout << fg::green << style::bold << "Current Substr: \t" << fg::reset
       << fgB::blue << word << fg::reset << style::reset << endl;
  cout << endl;
}


void errorMessage(string message) {
  cout << bgB::red << fgB::gray << message << fg::reset << bg::reset << endl;
  sleep(1);
}

int main(int argc, char **argv) {

    console_size = getConsoleSize();

    Timer T;                          // Create a timer
    char k;                           // Holds the character being typed
    vector<string> tempWords;         // Temp vector used in wordSearch function
    int iterations = 0;               // Holds integer value for amount of times wordSearch function is called
    string key;                       // Holds string value of matches from dictionary JSON file
    string substr = "";               // Var to concatenate letters to
    bool deleting = false;            // Bool to determine whether the deleting the last key pressed in 
                                      // the console
    int loc;
        
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

    vector<string> mainMenu;

    mainMenu.push_back("Type letters and watch the results change.");
    mainMenu.push_back(
      "Hit the Backspace key to erase a letter from your search string.");
     mainMenu.push_back(
      "When a single word is found, it's definition will be displayed.");

  clearConsole();
  titleBar("Getch Example", console_size.width);
  printMenu(mainMenu);

// While Z is not pressed in the terminal, loop through
while ((k = getch()) != 'Z') 
{
    clearConsole();
    titleBar("Getch Example", console_size.width);
    printMenu(mainMenu);

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
      substr += k; // append char to substr
    }

    horizontalBar();
    printCurrent(k, substr);
    
    T.Start();          // Start timer to see how long it takes to load Json File

    if ((int)k != 32) { // if k is not a space print it

      for (auto &c : substr)
        c = tolower(c);

        vector<string> matchingWords;
        string definition;

        // Iterate over all key-value pairs
        for (auto &element : myJson.items()) 
        {
            
            key = element.key();
            int pos = (int)key.find(substr);

            // Check if the key contains the partialKey substring
            // Found a match, do something with it
            if(key.find(substr) != string::npos && pos == 0)
            {
                // Filter out all words that doesn't begin with the substring
                matchingWords.push_back(key);
                
                if(matchingWords.size() == 1)
                {
                    // Display definition when last word left
                    definition = element.value();
                }
            }
            
        }    

        T.End();  //End timer to see how long it takes to load Json File

        cout << endl << matchingWords.size() << " words found in " <<
         (double)T.NanoSeconds() / 100000000 << " seconds" << endl;

        if(matchingWords.size() < 10)
        {
             for(int i = 0; i < matchingWords.size(); i++)
        {
            loc = matchingWords[i].find(substr);

            if (loc != string::npos) 
            {
                printHighlightedSubstr(matchingWords[i], substr, loc);
            }
        cout << " ";
            if(matchingWords.size() == 1)
            {
                cout << endl << definition << endl;
            }
        }
        } 
        else
        {
            for(int i = 0; i < 10; i++)
            {
                loc = matchingWords[i].find(substr);

            if (loc != string::npos) 
            {
                printHighlightedSubstr(matchingWords[i], substr, loc);
            }
                cout << matchingWords[i] << " ";
            }
        }
    }
}
    return 0;
}
