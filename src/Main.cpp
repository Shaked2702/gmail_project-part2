#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include "App.h"
#include "StreamIO.h"
#include "IIO.h"
#include "SocketIO.h"

using namespace std;
using namespace Command;

// Main function:
//1. Defines the input&output streams.
//2. Defines the data file paths.
//3. Defines the mapping of commands.
//Note: IMenu allocated in stack - approved scope of main always exists at the moment.
int main (int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <port> <setup_line>" << endl;
        return 1;
    }
    // Convert the argument to an integer
    int port = atoi(argv[1]);
    // Check if the conversion was successful
    if (port < 1024 || port > 49151) { // 1024-49151 are the legal ports
        cout << "Invalid Port Number. Please use a port between 1024 and 49151." << endl;
        return 1;
    }
    // Combine all remaining arguments into one setup string
    string setup;
    istringstream stream;
    // Check if there are more than 2 arguments
    if (argc > 2) {
       for (int i = 2; i < argc; ++i) {
            setup += argv[i];
            if (i < argc - 1) setup += " ";
        }
        stream.str(setup);       // Load the string into the stream
    }
    else {
        stream.str(" ");       // Load empty string into the stream
    }
    // Define the commands
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    commands[3] = new DeleteCommand();
    ConsoleMenu menu;
    //allocate in dynamicly heap
    SocketIO* Io = new Socket::SocketIO(port);

    
    // std::istream& input_stream = std::cin;
    // std::ostream& output_stream = std::cout;

    // Paths to the data files:
    // Set to work correctly for current working directory: src/exe/    Which is docker default cwd on our image.
    // So the string path to data is: "../../data/setup.txt"
    // If you are working locally and running g++ -o test Main.cpp x.cpp y.cpp ...
    // Change the path to: "../data/setup.txt"      (Same for url.txt)
    string const data_url_path = "../../data/url.txt";
    string const data_setup_path = "../../data/setup.txt";

    map<int,string> messages_output;
    messages_output[0] = "400 Bad Request\n";
    messages_output[1] = "201 Created\n";
    messages_output[2] = "201 Created\n";
    messages_output[3] = "200 Ok\n\nfalse\n";
    messages_output[4] = "200 Ok\n\ntrue true\n";
    messages_output[5] = "200 Ok\n\ntrue false\n";
    messages_output[6] = "200 Ok\n\nfalse true\n";
    messages_output[7] = "204 No Content\n";
    messages_output[8] = "404 Not Found\n"; 
    App app(menu, commands, Io, Io,
         data_url_path, data_setup_path, messages_output ,stream);

    // Run will stop if initialize ids failed.
    app.run();

    delete commands[1];
    delete commands[2];
    delete commands[3];
    delete Io;
}