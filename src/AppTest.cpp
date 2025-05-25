#include <gtest/gtest.h>
#include "LineSetup.h"
#include "IDataStructure.h"
#include "BloomFilter.h"
#include "ICommand.h"
#include "AddCommand.h"
#include "CheckCommand.h"
#include "DeleteCommand.h"
#include "App.h"
#include "IMenu.h"
#include "ConsoleMenu.h"
#include "ISetup.h"
#include "LineSetup.h"
#include "StreamIO.h"
#include "IIO.h"
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Setup;
using namespace Command;

    map<int, string> messages_output = {
        {0, "400 Bad Request\n"},
        {1, "201 Created\n"},
        {2, "201 Created\n"},
        {3, "200 Ok\n\nfalse\n"},
        {4, "200 Ok\n\ntrue true\n"},
        {5, "200 Ok\n\ntrue false\n"},
        {6, "200 Ok\n\nfalse true\n"},
        {7, "204 No Content\n"},
        {8, "404 Not Found\n"}
    };
/*-------------------------------------------------AppSuite------------------------------------------*/

// Test the usage of the App constructor
TEST(App, ConstructorTest) {
    // Create IMenu object
    ConsoleMenu menu;
    // Create a LineSetup object
    LineSetup lineSetup;
    // Create a map to hold commands
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    
    // Create a setup stream
    istringstream input_setup("8 2 1\n");

    // Create an input stream (fake cin)
    istringstream input_stream(""); // Empty

    // Create fake cin
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin

    // Create an output stream (cout)
    ostringstream output_stream;

    // Create fake cout
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    // Opens both cin and cout in our object
    int port_rand = rand() % 10000 + 1024; // Random legal port
    SocketIO* input_io = new SocketIO(port_rand); // Object stands for cin IO


    // Define the data file paths
    string data_url_path = "test_urls.txt";
    string data_setup_path = "test_setup.txt";

    // Create an App object
    App app(menu, commands, input_io, input_io,
            data_url_path, data_setup_path, messages_output, input_setup);
    EXPECT_TRUE(true); // If reach here without exception, constructor works.

    // Restore cin and cout
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout

    delete(input_io); // Clean up the io stream
}

// Test the usage of the App destructor
TEST(App, DestructorTest) {
    // Create IMenu object
    ConsoleMenu menu;
    // Create a map to hold commands
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();

    // Create a setup stream
    istringstream input_setup("8 2 1\n");

    // Create an input stream
    istringstream input_stream("");
    // Create an output stream
    ostringstream output_stream;

    // Create fake cin
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    // Create fake cout
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    int port_rand = rand() % 10000 + 1024; // Random legal port
    SocketIO* io = new SocketIO(port_rand); // Object stands for cin IO

    // Define the data file paths
    string data_url_path = "test_urls.txt";
    string data_setup_path = "test_setup.txt";

    // Create an App object
    App* app = new App(menu, commands, io, io,
            data_url_path, data_setup_path, messages_output, input_setup);
  
    // Call the destructor
    //cout << "Ctor called.\n"; // debugging
    app->~App();
    // Check if the destructor works without any issues
    EXPECT_TRUE(true); // If reach here without exception, destructor works.

    // Restore cin and cout
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout

    delete io; // Clean up the io stream
}

// Test the usage of the Map of Commands
TEST(App, MapCommandsTest) {
    // Create IMenu object
    ConsoleMenu menu;
    // Create a map to hold commands
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();

    // Create a setup stream
    istringstream input_setup("8 2 1\n");

    // Create an input stream
    istringstream input_stream(""); // Empty input
    // Create an output stream
    ostringstream output_stream;

    ICommand* cmd1 = commands[1];
    ICommand* cmd2 = commands[2];
    // Check casting to AddCommand isn't returning nullptr
    EXPECT_NE(dynamic_cast<AddCommand*>(cmd1), nullptr);
    EXPECT_NE(dynamic_cast<CheckCommand*>(cmd2), nullptr);
}

/*--------------------------------------------------MainFileSuite-----------------------------------------*/
// Testing functionality of the App - Socket input based.      

// Test SocketIO Basic GetLine
TEST(Main, TestMainRunWith2Clients) {
    // Generate random port between 1024 and 49151
    srand(time(nullptr));  // Only call once per program
    int port = rand() % (49151 - 1024 + 1) + 1024;
    //cout << "Port: " << port << endl; // debugging
    //int port = 12345; // Fixed port for testing
    // Create a setup stream
    istringstream input_setup("8 2 1\n");
    // Need to fork so accept command inside ctor won't stop client creation.
    int pid = fork();
    int pid2 = 0;
    if (pid != 0) {
        pid2 = fork();
    }
    if (pid == 0) {
        // Waits 2seconds for parent to create server socket
        sleep(1);
        std::map<int, const char*> mesages_to_send = {
            {0, "POST www.example.com\n"}, {1, "1 www.example.com"}, {2, "DELETE www.example2.com"},
            {3, "GET www.example.com"}, {4, "DELETE www.example.com"},
            {5, "GET www.example.com"}, {6, "POST www.example.com"}
        };
        // Child (client) process
        int client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sock < 0) {
            perror("error creating socket");
        }
        // Set ip&port struct
        const char* ip_address = "127.0.0.1";
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(ip_address);
        sin.sin_port = htons(port);
        // Connect to server
        if (connect(client_sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("error connecting to server");
        }
        char buffer_recv[1024];
        //const char* buffer;
        int bytes_recv, bytes_sent, bytes_received;
       for( int i = 0; i < 7; i++) {
            // Send data to server
            //buffer = mesages_to_send[i].c_str();
            bytes_sent = send(client_sock, mesages_to_send[i], strlen(mesages_to_send[i]), 0);
            if (bytes_sent < 0) {
                perror("error sending data");
            }
        //cout << " message sent: " <<  mesages_to_send[i] << endl; // debugging
        // Receive creat message
        
        bytes_received = recv(client_sock, buffer_recv, sizeof(buffer_recv), 0);
        if (bytes_received < 0) {
            perror("error receiving data");
        }
        //cout << " Buffer recived: " << buffer_recv << endl; // debugging
        switch (i)
        {
            // first case, sending a valid add command
        case 0:
            EXPECT_STREQ(buffer_recv, "201 Created\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration
            break;

        // Second case, sending an unvalid command
        case 1:
            EXPECT_STREQ(buffer_recv, "400 Bad Request\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration

            break;

        // Third case, sending a delete to url that doesn't exist
        case 2:
            EXPECT_STREQ(buffer_recv, "404 Not Found\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration
            break;

        // Fourth case, sending a valid check command to url that exists
        case 3:
            EXPECT_STREQ(buffer_recv, "200 Ok\n\ntrue true\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration
            break;

        // Fifth case, Deleting example.com
        case 4:
            EXPECT_STREQ(buffer_recv, "204 No Content\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration
            break;

        // Sixth case, after deleting example.com, check if it exists and get false positive because delete dosent change the bloom filter
        case 5:
            EXPECT_STREQ(buffer_recv, "200 Ok\n\ntrue false\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration
            break;

        // Seventh case, sending a add command 
        case 6:
            EXPECT_STREQ(buffer_recv, "201 Created\n"); // Check if the received data matches the expected data
            memset(buffer_recv, 0, sizeof(buffer_recv)); // Clear the buffer for the next iteration
            break;  
        default:
            break;
        }
    }
        // Close client socket
        close(client_sock);
        //cout << "Client socket closed\n"; // debugging
        exit(0); // Exit child process
        //////////////////////////////second child process//////////////////////////////////////
    } else if (pid2 == 0) {
        sleep(2);
        int client2_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (client2_sock < 0) {
            perror("error creating socket");
        }
       
    // Set ip&port struct
    const char* ip2_address = "127.0.0.2";
    struct sockaddr_in sin2;
    memset(&sin2, 0, sizeof(sin2));
    sin2.sin_family = AF_INET;
    sin2.sin_addr.s_addr = inet_addr(ip2_address);
    sin2.sin_port = htons(port);
    // Connect to server
    if (connect(client2_sock, (struct sockaddr *) &sin2, sizeof(sin2)) < 0) {
        perror("error connecting to server");
    }
    char buffer_recv[1024];
    const char* mesages_to_send = "DELETE www.example.com";
    int bytes_sent = send(client2_sock, mesages_to_send, strlen(mesages_to_send), 0);
    if (bytes_sent < 0) {
        perror("error sending data");
    }
    int bytes_received = recv(client2_sock, buffer_recv, sizeof(buffer_recv), 0);
    if (bytes_received < 0) {
        perror("error receiving data");
    }
    EXPECT_STREQ(buffer_recv, "204 No Content\n");
    // Close client socket
    close(client2_sock);
    //cout << "Client socket closed\n"; // debugging
    exit(0); // Exit child process
}
/////////////////////////////////////////////////Parent Process//////////////////////////////////////
    else {
        map<int, ICommand*> commands;
        commands[1] = new AddCommand();
        commands[2] = new CheckCommand();
        commands[3] = new DeleteCommand();
        ConsoleMenu menu;
        SocketIO* Io = new Socket::SocketIO(port);

        std::string data_url_path = "../../data/url_test.txt";
        std::string data_setup_path = "../../data/setup_test.txt";
        
        std::map<int, std::string> messages_output = {
            {0, "400 Bad Request\n"}, {1, "201 Created\n"}, {2, "201 Created\n"},
            {3, "200 Ok\n\nfalse\n"}, {4, "200 Ok\n\ntrue true\n"},
            {5, "200 Ok\n\ntrue false\n"}, {6, "200 Ok\n\nfalse true\n"},
            {7, "204 No Content\n"}, {8, "404 Not Found\n"}
        };
        Setup::LineSetup lineSetup;
        IDataStructure* m_ids = lineSetup.InitializeIDS(data_setup_path, data_url_path, input_setup);
       
        Io->acceptClient();
        // Open the url file stream for the command
        IIO* url_file_stream = new StreamIO(data_url_path); // Open the URL file for appending - adding urls at end only.
        int task, message;
        for (int i = 0; i < 7; i++){
        ostringstream url_buffer_out; // Temporary stream to hold the Url from the Menu.
        task = menu.GetCommand(Io, url_buffer_out);
        //std::cout << "\n " << url_buffer_out.str() << std::endl;
        //cout << "task: " << task <<endl;
        // Convert stream of url buffer out, into url buffer in for the command.
        istringstream url_buffer_in(url_buffer_out.str());
        // When the istream is not interactive like "cin" (infinity input) - it will return 0 when stream is end.
        if (task == 0) {
            Io->write(messages_output[task]);
            continue;
        }

        // Use map to execute a command.
        message = commands[task]->execute(*m_ids ,url_file_stream, url_buffer_in);
        //cout << "message: " << message << " " << messages_output[message] << endl; // Debugging
        // Use map to print the output message into the IO Object
        Io->write(messages_output[message]);
        }
        waitpid(pid, nullptr, 0);
        Io -> acceptClient();
        ostringstream url_buffer_out; // Temporary stream to hold the Url from the Menu.
        task = menu.GetCommand(Io, url_buffer_out);
        //std::cout << "\n " << url_buffer_out.str() << std::endl;
        //cout << "task: " << task <<endl;
        // Convert stream of url buffer out, into url buffer in for the command.
        istringstream url_buffer_in(url_buffer_out.str());
        // When the istream is not interactive like "cin" (infinity input) - it will return 0 when stream is end.
        if (task == 0) {
            Io->write(messages_output[task]);
        }
        else {
        // Use map to execute a command.
        message = commands[task]->execute(*m_ids ,url_file_stream, url_buffer_in);
        Io->write(messages_output[message]);
        }
        waitpid(pid2, nullptr, 0);
        delete url_file_stream; // Clean up the URL file stream
        // Socket destructor will close the socket
        // Wait for child process to finish
        //cout << "Waiting for child process to finish\n"; // debugging
        //waitpid(pid, nullptr, 0);
        //cout << "App run finished\n"; // debugging
        remove("../../data/url_test.txt");
        remove("../../data/setup_test.txt");
        delete commands[1];
        delete commands[2];
        delete commands[3];
    }
}



/* NEXT TEST IN NOT-IN-USE: Testing functionality of the App - Consolse input based.
TEST(App, RunTest) {
    // Create IMenu object
    ConsoleMenu menu;

    // Define the data file paths
    string data_url_path = "test_urls.txt";
    string data_setup_path = "test_setup.txt";

    // Create a map to hold commands
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();

    // Create a setup stream
    istringstream input_setup("8 2 1\n");
    // Create an input stream
    istringstream input_stream("POST https://www.example.com\n GET   https://www.example.com\n");
    // Create an output stream
    ostringstream output_stream;
    
    // Create fake cin
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    // Create fake cout
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin IO
    IIO* io2 = new StreamIO(""); // Object stands for cin IO

    // Create an App object
    App app(menu, commands, io, io2,
            data_url_path, data_setup_path, messages_output, input_setup);

    app.run();

    // Check if the output stream contains expected results
    
    EXPECT_EQ(output_stream.str(), "201 Created\n200 Ok\n\ntrue true\n"); // Check if the output stream contains expected results

    // Restore cin and cout
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete io; // Clean up the io stream
}*/


/*--------------------------------------------------MainFileSuite-----------------------------------------*/
/*------------------------------SUITE NOT-IN-USE: USED TO TEST APP WITH CONSOLE INPUTS--------------------*/
/*
// Copies main functioning with a basic input - true positive test.
TEST(MainSprint1, BasicMainTrueTrue) {
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    ConsoleMenu menu;

    // Create a setup stream
    istringstream input_setup("8 2 1\n");
    // Note in main using reference because passing cin cout.
    std::istringstream input_stream("POST https://www.example.com\nGET https://www.example.com\n");
    std::ostringstream output_stream("");

    // Fake cin and cout
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin IO

    remove("url.txt");
    remove("setup.txt");
    string const data_url_path = "url.txt";
    string const data_setup_path = "setup.txt";

    App app(menu, commands, io, io,
        data_url_path, data_setup_path, messages_output, input_setup);
    app.run();

    delete commands[1];
    delete commands[2];

    EXPECT_EQ(output_stream.str(), "201 Created\n200 Ok\n\ntrue true\n"); // Check if the output stream contains expected results

    // Restore cin and cout
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete io; // Clean up the io stream
}

// Copies main functioning with a basic input - false positive test.
TEST(MainSprint1, BasicMainTrueFalse) {
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    ConsoleMenu menu;

    // Create a setup stream
    istringstream input_setup("1 2\n");
    // Note in main using reference because passing cin cout.
    std::istringstream input_stream("POST https://www.example.com\nGET https://www.example2.com\n");
    std::ostringstream output_stream("");

    // Fake cin and cout
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin IO

    remove("url.txt");
    remove("setup.txt");
    string const data_url_path = "url.txt";
    string const data_setup_path = "setup.txt";

    App app(menu, commands, io, io,
        data_url_path, data_setup_path, messages_output, input_setup);
    app.run();

    delete commands[1];
    delete commands[2];

    EXPECT_EQ(output_stream.str(), "201 Created\n200 Ok\n\ntrue false\n"); // Check if the output stream contains expected results

    // Restore cin and cout
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete io; // Clean up the io stream
}

// Copies main functioning with a basic input - false test.
TEST(MainSprint1, BasicMainFalse) { // In high probability
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    ConsoleMenu menu;

    // Create a setup stream
    istringstream input_setup("64 4 2 1\n");
    // Note in main using reference because passing cin cout.
    std::istringstream input_stream("POST https://www.example.com\nGET https://www.example2.com\n");
    std::ostringstream output_stream("");

    // Fake cin and cout
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin IO
    
    remove("url.txt");
    remove("setup.txt");
    string const data_url_path = "url.txt";
    string const data_setup_path = "setup.txt";

    App app(menu, commands, io, io,
        data_url_path, data_setup_path, messages_output, input_setup);
    app.run();

    delete commands[1];
    delete commands[2];

    EXPECT_EQ(output_stream.str(), "201 Created\n200 Ok\n\nfalse\n"); // Check if the output stream contains expected results

    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete io; // Clean up the io stream
}

// Test main functioning with complex input space.
TEST(MainSprint1, ComplexInputSpace) {
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    ConsoleMenu menu;

    // Create a setup stream
    istringstream input_setup("16 3 2 1\n");
    // Note in main using reference because passing cin cout.
    std::istringstream input_stream("     POST       https://www.example.com     \n    GET    https://www.example.com    \n ");
    std::ostringstream output_stream("");
    
    // Fake cin and cout
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin IO

    remove("url.txt");
    remove("setup.txt");
    string const data_url_path = "url.txt";
    string const data_setup_path = "setup.txt";

    App app(menu, commands, io, io,
        data_url_path, data_setup_path, messages_output, input_setup);
    app.run();

    delete commands[1];
    delete commands[2];

    EXPECT_EQ(output_stream.str(), "201 Created\n200 Ok\n\ntrue true\n"); // Add fine, Check exist

    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete io; // Clean up the io stream
}

// Test main functioning many commands.
TEST(MainSprint1, MultipleCommandTest) {
    map<int, ICommand*> commands;
    commands[1] = new AddCommand();
    commands[2] = new CheckCommand();
    ConsoleMenu menu;

    // Create a setup stream
    istringstream input_setup("1024 4 2 1 1\n");
    // Note in main using reference because passing cin cout.
    std::ostringstream builder; // Temp stream to build the input
    builder << "";
    for (int i = 0; i < 20; i++) {
        builder << "POST https://www.example" << i << ".com\n";
    }
    for (int i = 0; i < 20; i++) {
        builder << "GET https://www.example" << i << ".com\n";
    }

    std::istringstream input_stream(builder.str());  // Now ready to read from

    std::ostringstream output_stream("");

    // Fake cin and cout
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input_stream.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output_stream.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin IO
    
    remove("url.txt");
    remove("setup.txt");
    string const data_url_path = "url.txt";
    string const data_setup_path = "setup.txt";

    App app(menu, commands, io, io,
        data_url_path, data_setup_path, messages_output, input_setup);
    app.run();

    delete commands[1];
    delete commands[2];
    string expected_output = "201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n201 Created\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n200 Ok\n\ntrue true\n";
    EXPECT_EQ(output_stream.str(), expected_output); // Check if the output stream contains expected results

    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete io; // Clean up the io stream
}
*/


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    // Delete .txt files after running all tests - Created inside test only.
    remove("test_urls.txt");
    remove("test_setup.txt");
    remove("url.txt");
    remove("setup.txt");   
    return result;
    // Splitted the return and test to delete files after tests.
}