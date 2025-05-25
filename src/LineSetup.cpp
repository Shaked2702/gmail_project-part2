

#include "LineSetup.h"
#include "BloomFilter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "StreamIO.h"
#include "IIO.h"
using namespace std;

namespace Setup {
    
// Check if the number is valid (positive integer).
bool LineSetup::IsValidNumber(const std::string& str) const {
    return std::regex_match(str, std::regex("^[1-9][0-9]*$"));
}

// Check if the file exists.
bool LineSetup::FileExists(const std::string& fileName) const {
    std::ifstream file(fileName);
    return file.good(); // Check if the file can be opened
}

// Parse the input string and check that the string is all positive numbers.
bool LineSetup::ParseInput(const std::string& input, std::vector<int>& iter_hash, size_t& array_size) const {
    std::istringstream lineStream(input); // Create a string stream from the input string
    std::string token;
    iter_hash.clear(); // Clear the vector to store hash function iterations
    bool firstInput = true; // Flag to check if it's the first input
    

    // A loop that splits the input string into tokens and checks if they are valid numbers.
    while (lineStream >> token) {
        if (IsValidNumber(token)) {
            if(firstInput) {
                array_size = std::stoi(token); // First number is the array size
                firstInput = false; // Set the flag to false after the first input
            } else {
                iter_hash.push_back(std::stoi(token)); // Convert to integer and add to vector
            }
        } else {
            return false; // Invalid input
        }
    }

    // Return true if the vector of hash function iterations is not empty and the array size is greater than 0.
    return !iter_hash.empty() && array_size > 0;
}


IDataStructure* LineSetup::InitFromInput(const string& setupFile, istream& input_stream) {
    string input; // Variable to store user input
    vector<int> iter_hash; // Vector to store hash function iterations
    size_t array_size = 0; // Variable to store array size

    getline(input_stream, input); // Read the entire line of input
        
        // Parse the input and check if it's valid
        if (ParseInput(input, iter_hash, array_size)) {
            // Create a new file named setupFile and write the input to it
            ofstream out(setupFile); // Note: Truncate the existing data - which is for now good - no data.
            out << input << "\n";
            out.close(); // Close the file
            // Create a new BloomFilter object with the parsed parameters
            IDataStructure* bloomFilter = new BloomFilter(iter_hash, array_size);
            return bloomFilter; // Return the initialized BloomFilter object    
        //} else { //debugging
            // If the input is invalid, prompt the user to enter it again - debugging
          //  cout << "Invalid input. Please enter a positive array size followed by hash function indices.\n"; //debugging
    } else {
        // If the input is invalid, prompt the user to enter it again
        cout << "Invalid input. Please enter a valid setup line.\n"; //debugging
        return nullptr; // Return nullptr so the construct should fail.
    }
}

void LineSetup::addUrlFromFile(IDataStructure& ids, const std::string& urlFile){
    LineSetup lineSetupInstance;
    if (lineSetupInstance.FileExists(urlFile)) { // Check if the file exists
        ifstream file(urlFile); // Open the file for reading
        string url;
        // Read each line from the file and add it to the data structure
        while (getline(file, url)) {
            ids.AddUrl(url); // Add the URL to the data structure
        }
        file.close(); // Close the file
    }
}

IDataStructure* LineSetup::InitializeIDS(const std::string& setupFile, const std::string& urlFile, istream& input_stream) {
    vector<int> iter_hash; // Vector to store hash function iterations
    size_t array_size = 0; // Variable to store array size

    // Check if the file exists and has a valid format
    if (FileExists(setupFile)) {
        ifstream file(setupFile); // Open the file for reading
        string line;
        getline(file, line); // Read the first line of the file
        file.close(); // Close the file

        // Parse the input from the file and check if it's valid
        if (ParseInput(line, iter_hash, array_size)) {
            // Create a new BloomFilter object with the parsed parameters
            IDataStructure* bloomFilter = new BloomFilter(iter_hash, array_size);
            // Add URLs from the file to the BloomFilter object
            addUrlFromFile(*bloomFilter, urlFile); // Add URLs from the file to the BloomFilter object
            return bloomFilter; // Return the initialized BloomFilter object
        } else {
            return(InitFromInput(setupFile, input_stream)); // If the file data is invalid, prompt for command line input
        }
    } else {
        return(InitFromInput(setupFile, input_stream)); // If the file doesn't exist, prompt for command line input
    }
    
}

}
