#include "DeleteCommand.h"
#include "StreamIO.h"
#include "IIO.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

namespace Command {
    // Delete class constructor
    DeleteCommand::DeleteCommand() {}

    // Delete class destructor
    DeleteCommand::~DeleteCommand() {}

    // Check if a command got valid - non empty, non whitespace.
    // Allows spaces at the beginning and end of the URL and returns the URL without them.
    // Return empty string for the invalid command.
    string DeleteCommand::isValidUrl(istream& input_stream) {
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
    int DeleteCommand::execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream) {
        string url = DeleteCommand::isValidUrl(input_stream);
        if (url == "") {
            //cout << "Invalid command" << endl; // Invalid input
            return 0;
        }
        // Url is valid
        //cout << "url inside execute func after validation: " << url << endl; //debugging
        url = DeleteCommand::ToLowerCase(url); // Convert URL to lower case.

        // Check if URL exist in the data structure (also in file) = true true
        if(ids.CheckUrl(url) == 1) {
            ids.DeleteUrl(url); // Delete URL from the data structure
            url_file_io->deleteline(url);// Delete URL from the file (respects non null terminated line).
            return 7; // Return 7 for success & URL was exist before.  
            //any other case means URL was not exist in the file.
        } else {
            return 8; // Url not found in the data structure
        }
    }


    string DeleteCommand::ToLowerCase(string& url) {
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



    // NOT IN USE
    // void DeleteCommand::FileDeleteUrl(string& url) {
    //     string filePath = "../../data/url.txt"; // Path to the URL file
    //     string line;        
    
    //     // Create a temp file to store the new content
    //     ofstream tempFile("temp.txt");
    //     // If opening the temp file failed, return
    //     if (!tempFile) {
    //         cerr << "Error opening temp file for writing.\n";
    //         return;
    //     }
    
    //     // Write to temp file all lines except the line with the url to delete
    //     while (getline(inFile, line)) {
    //         if (line != url) {
    //             tempFile << line << '\n'; // Write lines that don't match the URL
    //         }
    //     }
    
    //     // Close both files
    //     inFile.close();
    //     tempFile.close();
    
    //     // Remove the original file and rename the temp file to the original file name
    //     if (remove(filePath.c_str()) != 0 || rename("temp.txt", filePath.c_str()) != 0) {
    //         cerr << "Error replacing original file.\n";
    //     }
    // }
    
}