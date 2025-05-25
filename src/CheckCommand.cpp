#include "StreamIO.h"
#include "IIO.h"
#include "CheckCommand.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

namespace Command {
    // Check class constructor
    CheckCommand::CheckCommand() {}

    // Check class destructor
    CheckCommand::~CheckCommand() {}

    // Check if a command got valid - non empty, non whitespace.
    // Allows spaces at the beginning and end of the URL and returns the URL without them.
    // Return empty string for the invalid command.
    string CheckCommand::isValidUrl(istream& input_stream) {
        // Check if string is empty.
        if (input_stream.peek() == EOF) {
            //cout << "cin is empty\n"; //debugging
            return ""; // Invaild input
        }
        //cout << "cin has something\n"; //debugging

        string url;
        getline(input_stream, url);
        //cout << "url: " << url << endl; //debugging

        // Verify if the URL is valid
        regex urlPattern(R"(^\s*((https?|HTTPS?)://)?([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,}([a-zA-Z0-9-./%&=]*)?\s*$)");
        if (!regex_match(url, urlPattern)) {
            //cout << "URL is invalid by regex.\n"; // debugging
            return ""; // Invalid unput
        }
        //cout << "URL is valid.\n"; //debugging

        // Remove leading and trailing whitespaces
        istringstream stringstream(url);
        string new_url;
        stringstream >> new_url; // Read the URL without leading/trailing spaces
        //cout << "old_url: " << url << endl; //debugging
        //cout << "new_url: " << new_url << endl; //debugging
        return new_url;
    }

    // Execute function to check if the URL is in the data structure.
    int CheckCommand::execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream) {
        // cout << "Entered EXECUTE OF CHECKCOMMAND" << endl; // Debugging
        string url = CheckCommand::isValidUrl(input_stream); // Set's "" empty string if invalid URL.
        
        if (url == "") {
            //cout << "Invalid command" << endl; // Invalid input
            return 0;
        }
        url = CheckCommand::ToLowerCase(url); // Convert URL to lower case.
        // Url is valid
        int val = ids.CheckUrl(url); // Check URL in the data structure
        
        // Return value 0-false false, 1-true true, 2-true false, 3-false true.
        // Note: "3-false true" not exists for Bloom Filter.
        // 'Break' implemented as return command.
        switch (val) {
            case 0:
                return 3;
                // return false; // Equivalent of part1 "false"
            case 1:
                return 4;
                // return true; // Equivalent of part1 "true true"
            case 2:
                return 5;
                // return false; // Equivalent of part1 "true false"
            case 3:
                // Code can't reach here for Bloom Filter.
                return 6;
                // return true; // like "false true"
        }
        return 0; // Set default return value for compilation.
    }


    string CheckCommand::ToLowerCase(string& url) {
        size_t index=0;
        //check if start with "https://" or "HTTPS://"
        if(url.find("https://") == 0 || url.find("HTTPS://") == 0) {
           //replace "HTTP://" with "http://"
           url.replace(0, 8, "https://");
           index=8;
        }
        //check if start with "http://" or "HTTP://"
        if(url.find("http://") == 0 || url.find("HTTP://") == 0) {
            //replace "HTTP://" with "http://"
            url.replace(0, 7, "http://");
            index=7;
        }
        
        //find the first '/' after the "https://"
        size_t pos = url.find('/', index);
        //check if '/' found
        if (pos == std::string::npos) {
           // If not found, set pos to the end of the string
           pos = url.length(); 
        }
        for(int i=index; i<pos; i++) {
           //if letter convert to lower case.
            if (isalpha(url[i])) {
                url[i] = tolower(url[i]);
            }
        }

        return url; // Return the modified URL
    }
}