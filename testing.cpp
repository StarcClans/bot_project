#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include "logic.h"

using namespace std;


int main()
{
    string user_name;
    string user_input;
    const string filePath = "verbs.txt";

    // Declare a string array
    string allverblist[] = {"AuxVerb", "RAV", "QUA", "RepeatVerb", "non-repeat", "subject"};

    // there will be a function that will add more verbs, pronouns etc... depending on the user input that is not recognized by the bot
    // it use will be implemented as the user specified in the example of order and category of every word int the input/sentence

    int b = sizeof(allverblist) / sizeof(allverblist[0]);

    // Declare and initialize vectors using a loop
    vector<vector<string>> allverbVectors(b);
    
    for (int i = 0; i < b; ++i)
    {
        allverbVectors[i] = readLineFromFile(filePath, i);
    }


    //------------ New CLI right here (unfinished)--------------------------------

    // will rework the display a bit I will be adding this to github.com/starcclans/  stay tunned

    // sentences are recognized as > word word (sentence 1), word word word (sentence 2). word word (sentence 3)? and so on

    cout << "Xenon > What is your name?" << endl << " > ";
    cin >> user_name;

    cout << endl;

    BotRunner(user_input, allverbVectors, user_name);

    SavedFiles(); // saved user files with chat logs profile description

    return 0;
}
