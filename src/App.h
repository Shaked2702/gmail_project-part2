#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "ISetup.h"
#include "LineSetup.h"
#include "IDataStructure.h"
#include "ICommand.h"
#include "CheckCommand.h"
#include "AddCommand.h"
#include "IMenu.h"
#include "ConsoleMenu.h"
#include "StreamIO.h"
#include "IIO.h"
#include "SocketIO.h"
#include "DeleteCommand.h"

using namespace std;
using namespace Command;
using namespace Socket;

    class App {
        private:
            IDataStructure* m_ids; // Pointer to the data structure
            IMenu& m_menu; // Menu of application
            map<int, ICommand*> m_commands; // Map of commands
            map<int, string> m_messages_output; // Map of messages output
            // Note: Could be implemented with an array also - integer indexes.
            SocketIO* m_input_stream; // Input stream for console
            SocketIO* m_output_stream; // Output stream for console
            // Next two paths may be replaced with streams - in synchronization with line setup.
            string m_data_url_path; // Path to the data URL file
            string m_data_setup_path; // Path to the data setup file

        public:
            // Constructor
            App(IMenu& menu, map<int, ICommand*> commands, SocketIO* input_stream, SocketIO* output_stream,
                const string& data_url_path, const string& data_setup_path, map<int, string> messages_output, istream& setup_line);

            // Destructor
            ~App();

            // Run function holds the main loop of the application
            void run();
    };

#endif // APP_H