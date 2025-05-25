#include "AddCommand.h"
#include "StreamIO.h"
#include "IIO.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

namespace Command {
    // Add class constructor
    AddCommand::AddCommand() {}

    // Add class destructor
    AddCommand::~AddCommand() {}

    // Check if a command got valid - non empty, non whitespace.
    // Allows spaces at the beginning and end of the URL and returns the URL without them.
    // Return empty string for the invalid command.
    string AddCommand::isValidUrl(istream& input_stream) {
        // Check if string is empty.
        if (input_stream.peek() == EOF) {
            //cout << "cin is empty\n"; //debugging
            return ""; // Invaild input
        }
        //cout << "cin has something\n"; //debugging

        string url;
        getline(input_stream, url);
        //cout << "url inside isvalidurl func: " << url << endl; //debugging

        // Verify if the URL is valid
        regex urlPattern(R"(^\s*((https?|HTTPS?)://)?([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,}([a-zA-Z0-9-./%&=]*)?\s*$)"
);
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


    // Output mapping values: 0 = illegal command (non-valid url)
    // Execute function to add URL to the data structure.
    int AddCommand::execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream) {
        string url = AddCommand::isValidUrl(input_stream);
        if (url == "") {
            //cout << "Invalid command" << endl; // Invalid input
            return 0;
        }
        // Url is valid
        //cout << "url inside execute func after validation: " << url << endl; //debugging
        url = AddCommand::ToLowerCase(url); // Convert URL to lower case.

        if(ids.AddUrl(url)) { // Add URL to the data structure
            string url_newline = url + "\n"; // Add newline to the URL
            url_file_io->write(url_newline); // Add URL to the file
            return 1; // Return 1 for success & URL was not exist before.
        } else {
            // URL was exist in the Bloom Filter
            return 2; // Return 2 for legal command but URL was exist before.
        }
        // Note: Url added to file iff added to data structure - prevent duplicates.
    }


    string AddCommand::ToLowerCase(string& url) {
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