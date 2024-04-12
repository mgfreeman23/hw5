#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <set>
#include <string>
#endif

#include "wordle.h"
#include "dict-eng.h"

using namespace std;


// Add prototypes of helper functions here
void floatingSet(const string& in, multiset<char>& floaters, string curr, int numDashes, int numFloaters, int index, set<string>& resultSet, const std::set<std::string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    
    //initialize string of dashes (empty)
    std::string current(in.length(), '-');
    // convert floating letters to set of chars
    multiset<char> floaters;
    for (char letter : floating) {
        floaters.insert(letter);
    }
    // determine number of blank spaces in input string
    int numDashes = 0;
    for(int i = 0; i < in.length(); i++){
      if(in[i] == '-'){
        numDashes++;
      }
    }
    // initialize empty results set
    std::set<std::string> resultSet;

    //run helper function
    floatingSet(in, floaters, current, 0, numDashes, floaters.size(), resultSet, dict);
    
    // return final result set of strings
    return resultSet;

}

// generating all partially filled string combos using given floating letters
void floatingSet(const string& in, multiset<char>& floaters, string curr, int index, int numDashes, int numFloaters, set<string>& resultSet, const std::set<std::string>& dict) {
    // if end of word reached
    if (index == in.length()) {
        // make sure word is in the dictionary
        if(dict.find(curr) == dict.end()){
            return;
        }
        // add word to result set
        resultSet.insert(curr);
        return;
    }
    // letter found, not blank space
    if(in[index] != '-'){
      curr[index] = in[index];
      // recurse
      floatingSet(in, floaters, curr, index + 1, numDashes, numFloaters, resultSet, dict);
    }
    // blank space found 
    else {
      // if more floaters than blank space, impossible case
      if (numFloaters > numDashes){
        return;
      }
      // if floaters will fill in all the blank space
      if(numFloaters == numDashes){
        // iterate through all the floaters only testing them
        multiset<char>::iterator it;
        for(it = floaters.begin(); it != floaters.end(); ++it){
          curr[index] = *it;
          floaters.erase(it);
          floatingSet(in, floaters, curr, index + 1, numDashes - 1, numFloaters - 1, resultSet, dict);
          // reinsert
          floaters.insert(*it);

        } 
      }
        // number of floaters less than the number of blank space 
        else if (numFloaters < numDashes){
          //iterate through floaters first
          multiset<char>::iterator it2;
          for(it2 = floaters.begin(); it2 != floaters.end(); ++it2){
            curr[index] = *it2;
            floaters.erase(it2);
            floatingSet(in, floaters, curr, index + 1, numDashes - 1, numFloaters - 1, resultSet, dict);
            floaters.insert(*it2);
          }
          // iterate through letters of alphabet
          for(int i = 97; i < 123; i++){
            curr[index] = char(i);
            floatingSet(in, floaters, curr, index + 1, numDashes - 1, numFloaters, resultSet, dict);
          }
        }
      }
}




