#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <algorithm>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "logic.h"
#include <map>

using namespace std;

template<typename T>
vector<T>clearVector(vector<T>& vec)
{
    vec.clear();
    return vec;
}

int manageMood(const string& user_input, int currentMood)
{
    int decreaseMultiplier = 20; // 20% decrease for each word in all caps
    int maxMood = 100;

    // Check if the user input contains all caps
    size_t found = user_input.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    while (found != string::npos)
    {
        currentMood -= decreaseMultiplier;
        found = user_input.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ", found + 1);
    }

    // Ensure mood does not go below 0
    currentMood = max(0, currentMood);

    // Check for offensive language and increase or decrease mood accordingly

    if (currentMood >= maxMood)
    {
        // Initiate madness when reaching 100%
        // Would impliment roasting but not yet
    }

    return currentMood;
}


void SavedFiles()
{
    // Path to the current executable's directory
    filesystem::path current_path = filesystem::current_path();

    // Iterate over the files in the current directory
    for (const auto& entry : filesystem::directory_iterator(current_path))
    {
        // Check if it's a regular file and the name matches the pattern
        if (entry.is_regular_file() && (
        (entry.path().filename().stem().string()[0] >= 'A' && entry.path().filename().stem().string()[0] <= 'Z') &&
        (entry.path().filename().stem().string()[1] >= 'a' && entry.path().filename().stem().string()[1] <= 'z')
        ) && entry.path().extension() == ".txt")
        {
            cout << entry.path().filename() << endl;
        }
    }
}

string getCurrentTime() // will be used to determine output hello according to the current time
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
    return buffer;
}

string inverse(const string& pronoun) // inverts pronouns for response bot
{
    map<string, string> pronounMap = {
        {"i", "you"},
        {"you", "I"},
        {"your", "my"},
        {"he", "she"},
        {"she", "he"},
        {"it", "it"},
        {"we", "you"},
        {"they", "you"},
        {"i'm", "you're"},
        {"the", "the"},
        {"yours", "mine"},
        {"yourself", "myself"}
    };

    // Check if the pronoun exists in the map
    if (pronounMap.find(pronoun) != pronounMap.end())
    {
        return pronounMap[pronoun]; // Return the corresponding response
    }
    else
    {
        return pronoun; // Return unknown if the pronoun is not found
    }
}


vector<vector<string>> convertToSentences(const string& userInput)
{
    vector<vector<string>> stringArrays;
    vector<string> currentSentence;

    // Tokenize input into words
    istringstream iss(userInput);
    string word;

    while (iss >> word)
    {
        // If "," is found, start a new vector for the next sentence
        if (word == "," || word == "." || word == "?" || word == "!")
        {
            if (!currentSentence.empty())
            {
                stringArrays.push_back(currentSentence);
                currentSentence.clear();
            }
        }
        else
        {
            // Add word to the current vector
            word = inverse(word);
            currentSentence.push_back(word);
        }
    }

    // Add the last sentence (if any)
    if (!currentSentence.empty())
    {
        stringArrays.push_back(currentSentence);
    }

    return stringArrays;
}

void writeToFile(const string& input, const string& filename)
{
    ofstream outputFile(filename, ios::app); // Open the file in append mode

    if (!outputFile.is_open())
    {
        cout << "Error: Unable to open file " << filename << endl;
        return;
    }

    // Write the entire string to the file without iterating over its characters
    outputFile << input;

    outputFile << " ++ " + getCurrentTime() << endl; // Display time when typed

    outputFile.close();
}

vector<string> readLineFromFile(const string& filePath, int lineNumber)
{
    vector<string> words; // Vector to store words

    // Open the file
    ifstream inputFile(filePath);

    // Check if the file is open
    if (!inputFile.is_open())
    {
        cerr << "Error: Unable to open the file." << endl;
        // Returning an empty vector in case of an error
        return words;
    }

    // Read the specified line
    string line;
    for (int i = 0; i <= lineNumber; ++i)
    {
        getline(inputFile, line);
        if (i == lineNumber)
        {
            // Tokenize the line and store words in the vector
            istringstream lineStream(line);
            string word;
            while (lineStream >> word) {
                words.push_back(word);
            }
        }
    }

    // Close the file
    inputFile.close();

    // Return the vector of words
    return words;
}


vector<int> compareAndOutput(const vector<vector<string>>& sentences, const vector<vector<string>>& allverbVectors)
{
    vector<int> result;
    int sentenceIndex = 0;

    for (size_t i = 0; i < sentences.size(); ++i)
    {
        const vector<string>& sentence = sentences[i];

        for (size_t j = 0; j < sentence.size(); ++j)
        {
            bool found = false;

            for (size_t k = 0; k < allverbVectors.size(); ++k)
            {
                const vector<string>& subvector = allverbVectors[k];
                for (size_t m = 0; m < subvector.size(); ++m)
                {
                    if (sentence[j] == subvector[m])
                    {

                        result.push_back(sentenceIndex + 1); // Index of the sentence
                        result.push_back(j + 1); // Index of the sentence member
                        result.push_back(k + 1); // Index of the subvector

                        found = true;
                        break; // Break out of the innermost loop once the word is found
                    }

                }
                if (found)
                {
                    break; // Break out of the loop once the word is found
                }
            }
            if (!found)
            {
                result.push_back(sentenceIndex + 1); // Indicate no match found with the same sentence index
                result.push_back(j + 1); // Indicate no match found with the same sentence member index
                result.push_back(0);     // Indicate no match found with 0 as the vector index
            }
        }
        sentenceIndex++; // Increment sentence index after processing each sentence
    }

    return result;
}


vector<vector<int>> formatOutput(const vector<int>& outputArray)
{
    vector<vector<int>> formattedOutput;
    vector<int> currentRow;
    
    for (size_t i = 0; i < outputArray.size(); i += 3)
    {
        if (i > 0 && outputArray[i] != outputArray[i - 3])
        {
            formattedOutput.push_back(currentRow);
            currentRow.clear();
        }
        currentRow.push_back(outputArray[i + 2]);
    }
    if (!currentRow.empty())
    {
        formattedOutput.push_back(currentRow);
    }
    
    return formattedOutput;
}


// Function to compare the formatted output string with specific patterns
int comparePattern(const vector<int>& row)
{
    int pattern = 0; // Default pattern
    
    if (row.size() >= 2)
    {
        // Convert int to string for comparison
        int patternStr = row[0]*100 + row[1]*10 + row[2];

        switch (patternStr)
        {
            case 160:
            case 260:
                pattern = 1;
                break;

            case 316:
                pattern = 2;
                break;
            case 560:
            case 460:
                pattern = 3;
                break;
            case 610:
                pattern = 4;
                break;
            default:
                pattern = 0;
                break;
        }
    }

    return pattern;
}

vector<string> botresponse(int pattern, const string& user_name, const vector<vector<string>>& sentences)
{
    vector<string> response;

    for (const auto& sentence : sentences)
    {
        vector<string> rearrangedSentence(sentence.size()); // Initialize a vector to store the rearranged sentence
        string rearrangedStr; // Move the string inside the loop

        switch(pattern)
        {
            case 0:
                response.push_back("Sorry to tell you man, but I don't understand what you typed");
                break;
            case 1:
                rearrangedSentence[0] = sentence[1];
                rearrangedSentence[1] = sentence[0];
                for (size_t word = 2; word < sentence.size(); ++word)
                {
                    rearrangedSentence[word] = sentence[word];
                }

                for (const auto& word : rearrangedSentence)
                {
                    rearrangedStr += word + " "; // Concatenate each word inside the loop
                }
                response.push_back(rearrangedStr);
                rearrangedStr="";
                break;
            case 2:
                rearrangedSentence[0] = sentence[1];
                rearrangedSentence[1] = sentence[0];
                for (size_t word = 2; word < sentence.size(); ++word)
                {
                    rearrangedSentence[word] = sentence[word];
                }

                for (const auto& word : rearrangedSentence)
                {
                    rearrangedStr += word + " "; // Concatenate each word inside the loop
                }
                response.push_back(rearrangedStr);
                rearrangedStr="";
                break;
            case 3:
                rearrangedSentence[0] = sentence[1];
                rearrangedSentence[1] = sentence[0];
                for (size_t word = 2; word < sentence.size(); ++word)
                {
                    rearrangedSentence[word] = sentence[word];
                }

                for (const auto& word : rearrangedSentence)
                {
                    rearrangedStr += word + " "; // Concatenate each word inside the loop
                }
                response.push_back(rearrangedStr);
                rearrangedStr="";
                break;
            case 4:
                rearrangedSentence[0] = sentence[1];
                rearrangedSentence[1] = sentence[0];
                for (size_t word = 2; word < sentence.size(); ++word)
                {
                    rearrangedSentence[word] = sentence[word];
                }

                for (const auto& word : rearrangedSentence)
                {
                    rearrangedStr += word + " "; // Concatenate each word inside the loop
                }
                response.push_back(rearrangedStr);
                rearrangedStr="";
                break;
            default:
                response.push_back("I'm not sure how to respond to that.");
                break;
        }
    }

    return response;
}



void BotRunner(vector<vector<string>>& allverbVectors, const string& user_name)
{
    //srand(static_cast<unsigned>(time(0)));

    //unordered_map<string, string> userProfile;
    string user_input;
    string temp_string;
    string vectorNames[] = {"sentences", "formattedOutput", "result", "arrPattern", "arrPattern", "response"};


    vector<vector<string>> sentences;
    vector<vector<int>> formattedOutput;
    vector<int> result;
    vector<int> arrPattern;
    vector<string> response;

    bool endConversation = false;
    bool current_speaker = false;
    
    while(true)
    {

        if(!current_speaker)
        {
            // bot mechanics
            if(endConversation == true)
            {
                cout << "Xenon > " << "bye!" << endl;
                break;
            }

            for (const auto& pattern : arrPattern)
            {
                response = botresponse(pattern, user_name, sentences);

                for (const auto& resp : response)
                {
                    cout << "Xenon > " << resp + " " << endl;
                    temp_string += resp + " ";
                }
            }

            // Write the entire conversation to the file
            writeToFile(temp_string, user_name);

            current_speaker = true;

            // Clear the contents of vectors based on their names in the array
            clearVector(formattedOutput);
            clearVector(response);
            clearVector(result);
            clearVector(arrPattern);
            clearVector(sentences);

            temp_string.clear();
            user_input.clear();
        }
        else if(current_speaker)
        {
            cout << user_name << " > ";
            getline(cin, user_input);

            writeToFile(user_input, user_name);

            int moodPercentage = manageMood(user_input, moodPercentage); //calculate percentage mood

            transform(user_input.begin(), user_input.end(), user_input.begin(), ::tolower);
            
            sentences = convertToSentences(user_input);
            
            result = compareAndOutput(sentences, allverbVectors);

            // Format the output with new lines for each sentence
            formattedOutput = formatOutput(result);

            // Split the formatted output into lines and compare each line with specific patterns
            for (const auto& row : formattedOutput)
            {
                int pattern = comparePattern(row);
                arrPattern.push_back(pattern);
            }

            // Check if the user wants to end the conversation
            if (endConversation == false && user_input == "bye")
            {
                endConversation = true; // end conversation
            }

            // Update current_speaker based on the conversation flow
            current_speaker = false;
        }
    }
}
