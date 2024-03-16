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

void wordSearch(vector<string> &matchingWords, string& partialKey, int& iterations, bool deleting)
{
    vector<string> tempMWords;

    for(int i = 0; i < matchingWords.size(); i++)
    {
        string temp = matchingWords[i];
        if(temp[iterations] >= partialKey[iterations])
           {
                tempMWords.push_back(temp);
           }
     }   
    matchingWords = tempMWords;
    iterations++;
}

void errorMessage(string message) {
  cout << bgB::red << fgB::gray << message << fg::reset << bg::reset << endl;
  sleep(1);
}

int main(int argc, char **argv) {

    Timer T;   // create a timer
    char k; //holds the character being typed
    
    vector<string> tempWords;
    int iterations = 0;
    string key;
    string substr = "";              // var to concatenate letters to
    bool deleting = false;
    
    T.Start(); //Start timer to see how long it takes to load Json File

    // Load your JSON object as shown in previous examples
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

    //printf("Nanoseconds: %.17f\n", (double)T.NanoSeconds() / 1000000000);

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
      substr += k; // append char to substr
    }

    // Find any animals in the array that partially match
    // our substr word
    //matches = partialMatch(animals, substr);

    if ((int)k != 32) { // if k is not a space print it

      for (auto &c : substr)
        c = tolower(c);

        cout << substr << endl;
    

    // The substring you are looking for in the keys
   
       /*  if (argc == 1)
            partialKey = partialKey;
        else
            partialKey = argv[1];
 */
        vector<string> matchingWords;
        T.Start();
        // Iterate over all key-value pairs
        for (auto &element : myJson.items()) 
        {
            
            key = element.key();
            
            // Check if the key contains the partialKey substring
            //if (key.find(partialKey) != std::string::npos) 
            //{
            // Found a match, do something with it
            if(key.find(substr) != string::npos)
            {
                if(substr <= key)
                matchingWords.push_back(key);
                //std::cout << "Found partial match: " << key << std::endl;
            }
            
        }    

        for(int i = 0; i < substr.size(); i++)
        {
            wordSearch(matchingWords, substr, iterations, deleting);
        }
        T.End();

        cout << matchingWords.size() << " words found in " << (double)T.NanoSeconds() / 100000000 << " seconds" << endl;

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