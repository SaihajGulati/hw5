// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

//helper function that generates all possible combinations
void combos(std::string in,
    std::string floating,
    const std::set<std::string>& dict,
    std::set<std::string>& words);

//to return a set of indexes that are where thing shows up in in string
std::set<std::size_t> findOccurencces(std::string in, char thing);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> wordsList;
    combos(in, floating, dict, wordsList);
    return wordsList;

}

// Define any helper functions here

//helper function that finds all combinations of words
//in and floating are not references as they need to differ between recursive cases
//dict is a const reference as never changes
//words is a reference as it needs to be added to every case that finds a word, and we need to cumulative list to hold every word as that will allow us to return it in wordle
void combos(
    std::string in,
    std::string floating,
    const std::set<std::string>& dict,
    std::set<std::string>& words)
{
    //case where have whole word, so check if is english word and handle that case
    if (in.find("-") == std::string::npos)
    {
        if (dict.find(in) != dict.end())
        {
            words.insert(in);
        }
    }

    //otherwise handle case where have floating, and so need to fill those in first but try every combo in the empty spots
    //so, only deal with first floating and try with every spot in in, and let recursion handle rest of floating
    else if (floating.size() != 0)
    {
        std::set<std::size_t> places = findOccurencces(in, '-');
        std::set<std::size_t>::iterator it;

        //go through spots in dash and try combos with the letter in that spot
        for (it = places.begin(); it != places.end(); ++it)
        {
            //make copy of in so only one spot is changed
            std::string in2 = in;
            //make that spot in the in word equal to the first floating
            in2[*it] = floating[0];

            //make a new floating string that does include the used up floating on
            //std::string floatingcopy = floating.substr(0,i) + floating.substr(i+1,  std::string::npos);
            
            //recruse, with the one spot changed with the first from our floating string (hence in2, and hence passing in only the substring that does not include part of floating we just used)
            combos(in2, floating.substr(1,std::string::npos), dict, words);
        }
    }

    else //need to fill first empty space with a letter and try every single combination
    {
        size_t index = in.find("-");
        for(char d = 'a'; d <= 'z'; ++d)
        {
            //make copy of in so only one spot is changed
            std::string in2 = in;
            //make that spot in the in word equal to the first floating
            in2[index] = d;
            //recurse with our letter inserted
            combos(in2, floating, dict, words);
        }
    }
}

//finds occurences of thing in in, and returns a set with the indexes of in that have the thing
std::set<std::size_t> findOccurencces(std::string in, char thing)
{
    std::set<std::size_t> indices;
    for (size_t i = 0; i < in.size(); i++)
    {
        if (in[i] == thing)
        {
            indices.insert(i);
        }
    }
    return indices;
}