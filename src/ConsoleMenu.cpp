#include <iostream>
#include <fstream>
#include "ConsoleMenu.h"
#include "StreamIO.h"
#include "IIO.h"
#include <string>
#include <map>

using namespace std;

// Getting input from the console
// -1 for Error (Client disconnect or EOF) 0 for Wrong input, 1 for POST, 2 for GET, 3 for DELETE
int ConsoleMenu::GetCommand(IIO* input, ostream& output) {
    string UserInput;

    int response = input->getline(UserInput);
    if (response == 0) {
        return -1; // Socket disconnected or error occurred || EOF has reached
    }
    //cout << "UserInput: " << UserInput << endl; // Debugging - print the user input.
    if (input->eof()) {
        return -1; // End of file reached
    }
    // Ignore leading spaces in the string start
    while (!UserInput.empty() && isspace(UserInput[0])) {
        UserInput.erase(0, 1); // Remove leading space
    } 
    // If left with empty string - CheckInput will return false
    // Check if line starts with "1 " or "2 ", "3" and has something after it
    if (CheckInput(UserInput) == false) {
        output << "";
        return 0; // Invalid command
    }

    //substr(2)- 0 is the command number and 1 is the space between the command and the url.
    output << UserInput.substr(2) << endl; //output the url to the output stream.
    //cout << "SENT FROM CONSOLE MENU: " << UserInput.substr(2) << endl; //debugging
    return UserInput[0] - '0'; //returning the first char of the string as an int.
}
//checking if the input line is valid;
bool ConsoleMenu::CheckInput(string& UserInput) {
    //check where is the first space in the string.
    size_t space_pos = UserInput.find(' ');
    string command = (space_pos != string::npos) ? UserInput.substr(0, space_pos) : UserInput;
    //check if the command is valid.
    if (m_command_number.find(command) != m_command_number.end()) {
        // If found, replace the command in UserInput with the corresponding number
        UserInput = to_string(m_command_number[command]) + UserInput.substr(command.length()); // Replace with number
    } else {
        // If not found, return false
        return false; // Invalid command
    }
    //check if the line is valid.
    if (UserInput.length() >= 2) {
        return true;
    }
    return false;//if the input is not valid we return false.
}