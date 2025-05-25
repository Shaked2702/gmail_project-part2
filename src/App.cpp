#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "App.h"
#include "StreamIO.h"
#include "SocketIO.h"
#include "IIO.h"

using namespace std;

// Constructor for the App class - allocated m_ids dynamically (LineSetup)
App::App(IMenu& menu, map<int, ICommand*> commands, SocketIO* input_stream, SocketIO* output_stream,
    const string& data_url_path, const string& data_setup_path, map<int, string> messages_output, istream& setup_line)
    : m_menu(menu), m_commands(commands), m_input_stream(input_stream), m_output_stream(output_stream),
      m_data_url_path(data_url_path), m_data_setup_path(data_setup_path), m_messages_output(messages_output) {
    // Initialize the data structure
    Setup::LineSetup lineSetup;
    m_ids = lineSetup.InitializeIDS(m_data_setup_path, m_data_url_path, setup_line);
    // Allocated dynamically
}

// Destructor for the App class - deletes m_ids
App::~App() {
    delete m_ids; // Clean up the data structure
}

// Run function holds the main loop of the application
void App::run() {
    if (m_ids == nullptr) {
        return;
    }
    while (true) {
        // Create connection with a client
        //cout << "Before accept client" << endl; // Debugging
        m_input_stream->acceptClient();
        //cout << "After accept client" << endl; // Debugging

        while (true) {
            // Create url buffer stream for the Menu
            // Note url_buffer_out and url_buffer_in are scope inside loop - Restore to empty on each iteration.
            ostringstream url_buffer_out; // Temporary stream to hold the Url from the Menu.

            // cout << "Before get command" << endl;
            int task = m_menu.GetCommand(m_input_stream, url_buffer_out);
            // cout << "After get command" << endl;

            // Client disconnected or error occurred - Exit inner loop
            if (task == -1) {
                break;
            }

            // Convert stream of url buffer out, into url buffer in for the command.
            istringstream url_buffer_in(url_buffer_out.str());
            
            // When the istream is not interactive like "cin" (infinity input) - it will return 0 when stream is end.
            if (task == 0) {
                m_output_stream->write(m_messages_output[task]);
                continue;
            }

            // Open the url file stream for the command
            IIO* url_file_stream = new StreamIO(m_data_url_path); // Open the URL file for appending - adding urls at end only.

            // Use map to execute a command.
            int message = m_commands[task]->execute(*m_ids,url_file_stream, url_buffer_in);

            // Use map to print the output message into the IO Object
            m_output_stream->write(m_messages_output[message]);


            // Doesn't affect sockets. Generally for tests.
            if (m_input_stream->eof()) {
                break; // Exit the loop if the input stream is at EOF
            }

            delete url_file_stream; // Clean up the URL file stream
        }

        // Code after exiting a current client inner loop... (empty atm)
        // Check in tests maybe if not socket (dynamic_cast) to "break" again (the outer loop)
    }
    return; // Not important
}