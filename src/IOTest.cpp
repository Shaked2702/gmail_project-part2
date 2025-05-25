#include <gtest/gtest.h>
#include "IIO.h"
#include "StreamIO.h"
#include "SocketIO.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h> // For close()
#include <arpa/inet.h> // For inet_pton()
#include <netinet/in.h> // For sockaddr_in
#include <sys/socket.h> // For socket functions
using namespace std;
using namespace Socket;

/*------------------------------------StreamIO Suite-----------------------------------------*/

// Test Constructor and 2 hash functions with different iterations different values.
TEST(StreamIO, TestConstructor) {
    StreamIO* io = new StreamIO("");
    EXPECT_NE(io, nullptr);
    StreamIO* io2 = new StreamIO("test.txt");
    EXPECT_NE(io2, nullptr);
    delete io;
    delete io2;
    remove("test.txt");
}

// Test "write" to a file
TEST(StreamIO, TestWrite) {
    StreamIO* io = new StreamIO("test.txt");
    string input = "Hello World!\nGoodbye World!\n";
    io->write(input);
    delete io;

    string output;
    string output2;
    ifstream in("test.txt");
    getline(in, output);
    getline(in, output2);
    EXPECT_EQ("Hello World!", output);
    EXPECT_EQ("Goodbye World!", output2);
    remove("test.txt");
}

// Test "getline" from a file
TEST(StreamIO, TestGetLine) {
    ofstream out("test.txt");
    out << "Hello World!\nGoodbye World!\n";
    out.close();
    StreamIO io("test.txt");
    string output1;
    string output2;
    io.getline(output1);
    io.getline(output2);
    EXPECT_EQ("Hello World!", output1);
    EXPECT_EQ("Goodbye World!", output2);
    remove("test.txt");
}

// Test "getline" and "write" with a file - notice not at the same time
TEST(StreamIO, TestGetLineAndWrite) {
    StreamIO* io = new StreamIO("test.txt");
    string input = "Hello World!\nGoodbye World!\n";
    io->write(input);
    delete io;

    StreamIO io2("test.txt");
    string output;
    string output2;
    io2.getline(output);
    io2.getline(output2);
    EXPECT_EQ("Hello World!", output);
    EXPECT_EQ("Goodbye World!", output2);
    remove("test.txt");
}

// Test read and write with empty string
TEST(StreamIO, TestEmptyString) {
    StreamIO io("test.txt");
    string input = "";
    io.write(input);
    string output;
    io.getline(output);
    EXPECT_EQ(input, output);
    remove("test.txt");
}

// Next test will not work - TDD behavior wasn't needed.
// Future optional functioning to work with write and getline sametime.
// Test read and write many times - Behavior should read from beginning and write to EOF
/*TEST(StreamIO, TestMultipleReadWrite) {
    StreamIO io("test.txt");
    string input1 = "Hello World!\n";
    string input2 = "Goodbye World!\n";
    string input3 = "Hello Again!\n";
    string output1;
    string output2;
    string output3;
    io.write(input1);
    io.getline(output1);
    io.write(input2);
    io.write(input3);
    io.getline(output2);
    io.getline(output3);
    EXPECT_EQ(input1, output1);
    EXPECT_EQ(input2, output2);
    EXPECT_EQ(input3, output3);
    remove("test.txt");
}*/

// Test the deleteline function
TEST(StreamIO, TestDeleteLine) {
    ofstream out("test2.txt");
    out << "Hello World!\nGoodbye World!\nFFT_IS_REAL\n";
    out.close();
    StreamIO io("test2.txt");
    string output;
    io.getline(output);
    EXPECT_EQ("Hello World!", output);
    //cout << "Deleting line: " << output << endl;
    io.deleteline(output);
    string output2;
    io.getline(output2);
    //cout << "Next line: " << output2 << endl;
    EXPECT_EQ("Goodbye World!", output2);
    remove("test2.txt");
}

// Test deleteline comparing to the data inside file
TEST(StreamIO, TestDeleteLineFile) {
    ofstream out("test3.txt");
    out << "Hello World!\nGoodbye World!\nFFT_IS_REAL\n";
    out.close();
    StreamIO io("test3.txt");
    string output;
    io.getline(output);
    EXPECT_EQ("Hello World!", output);
    //cout << "Deleting line: " << output << endl;
    io.deleteline(output);
    string remaining;
    ifstream in("test3.txt");
    ostringstream buffer;
    buffer << in.rdbuf();
    remaining = buffer.str();
    in.close();
    EXPECT_EQ(remaining, "Goodbye World!\nFFT_IS_REAL\n");
    //cout << "Remaining content: " << remaining << endl;
    remove("test3.txt");
}

// Test delete multiple times
TEST(StreamIO, TestDeleteLineMultiple) {
    ofstream out("test4.txt");
    out << "Hello World!\nGoodbye World!\nFFT_IS_REAL\nDijkstra was right!\n";
    out.close();
    StreamIO io("test4.txt");
    string output;
    io.getline(output);
    EXPECT_EQ("Hello World!", output);
    //cout << "Deleting line: " << output << endl;
    io.deleteline(output);
    string output2;
    io.getline(output2);
    //cout << "Next line: " << output2 << endl;
    EXPECT_EQ("Goodbye World!", output2);
    //cout << "Deleting line: " << output2 << endl;
    io.deleteline(output2);
    string remaining;
    ifstream in("test4.txt");
    ostringstream buffer;
    buffer << in.rdbuf();
    remaining = buffer.str();
    in.close();
    EXPECT_EQ(remaining, "FFT_IS_REAL\nDijkstra was right!\n");
    //cout << "Remaining content: " << remaining << endl;
    remove("test4.txt");
}

// Test delete from middle of file
TEST(StreamIO, TestDeleteLineMiddle) {
    ofstream out("test5.txt");
    out << "Hello World!\nGoodbye World!\nFFT_IS_REAL\nDijkstra was right!\n";
    out.close();
    StreamIO io("test5.txt");
    string output = "FFT_IS_REAL";
    io.deleteline(output);
    io.close();
    string remaining;
    ifstream in("test5.txt");
    ostringstream buffer;
    buffer << in.rdbuf();
    remaining = buffer.str();
    in.close();
    EXPECT_EQ(remaining, "Hello World!\nGoodbye World!\nDijkstra was right!\n");
    //cout << "Remaining content: " << remaining << endl;
    remove("test5.txt");
}

// Test delete from last line of file
TEST(StreamIO, TestDeleteLineLast) {
    ofstream out("test6.txt");
    out << "Hello World!\nGoodbye World!\nFFT_IS_REAL\nDijkstra was right!\n";
    out.close();
    StreamIO io("test6.txt");
    string output = "Dijkstra was right!";
    io.deleteline(output);
    io.close();
    string remaining;
    ifstream in("test6.txt");
    ostringstream buffer;
    buffer << in.rdbuf();
    remaining = buffer.str();
    in.close();
    EXPECT_EQ(remaining, "Hello World!\nGoodbye World!\nFFT_IS_REAL\n");
    //cout << "Remaining content: " << remaining << endl;
    remove("test6.txt");
}

// Test delete from empty file
TEST(StreamIO, TestDeleteEmptyFile) {
    ofstream out("test7.txt");
    out.close();
    StreamIO io("test7.txt");
    string str = "Dijkstra was right!";
    EXPECT_EQ(io.deleteline(str), true);
    io.close();
    string remaining;
    ifstream in("test7.txt");
    ostringstream buffer;
    buffer << in.rdbuf();
    remaining = buffer.str();
    in.close();
    EXPECT_EQ(remaining, "");
    //cout << "Remaining content: " << remaining << endl;
    remove("test7.txt");
}

/*-----------------------------------------SocketIO Suite---------------------------------------*/
// Test SocketIO Basic GetLine
TEST(SocketIO, TestSocketIOGetLine) {
    // Generate random port between 1024 and 49151
    int port = rand() % (49151 - 1024 + 1) + 1024;

    // Need to fork so accept command inside ctor won't stop client creation.
    int pid = fork();
    if (pid == 0) {
        // Waits 2seconds for parent to create server socket
        sleep(1);
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
        // Send bytes
        const char* buffer = "Hello from client!";
        int bytes_sent = send(client_sock, buffer, strlen(buffer), 0);
        if (bytes_sent < 0) {
            perror("error receiving data");
        }
        // Close client socket
        close(client_sock);
        exit(0); // Exit child process
    } else {
        // Parent (server) process
        // Create server socket
        SocketIO serv_socket(port);
        serv_socket.acceptClient();
        // Set buffer to receive data
        string buffer;
        serv_socket.getline(buffer);
        EXPECT_EQ(buffer, "Hello from client!");
        // Socket destructor will close the socket
        // Wait for child process to finish
        waitpid(pid, nullptr, 0);
    }
}

// Test SocketIO Basic Write
TEST(SocketIO, TestSocketIOWrite) {
    // Generate random port between 1024 and 49151
    int port = rand() % (49151 - 1024 + 1) + 1024;

    // Need to fork so accept command inside ctor won't stop client creation.
    // This test uses child process to create a server and parent process is the client.
    // Test will be for the data the client receives from the server.
    int pid = fork();
    if (pid == 0) {
        // Child (server) process
        // Create server socket
        SocketIO serv_socket(port);
        serv_socket.acceptClient();
        // Wait for client to connect
        sleep(1);

        // Write
        string text = "Hello from server!";
        serv_socket.write(text);
        
        // Close server socket is by Destructor, no needed to close.
        exit(0); // Exit child process
    } else {
        // Parent (client) process
        // Wait for server to turn on.
        sleep(1);
        // Create client socket
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

        // Receive data
        char buffer[1024];
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            perror("error receiving data");
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received data

        // Check if the received data matches the expected data
        EXPECT_EQ(string(buffer), "Hello from server!");
        // Close client socket
        close(client_sock);

        // Wait for child process to finish
        waitpid(pid, nullptr, 0);
    }
}

// Test SocketIO Write And GetLine in UPPER CASE
TEST(SocketIO, TestSocketIOWriteAndGetLine) {
    // Generate random port between 1024 and 49151
    int port = rand() % (49151 - 1024 + 1) + 1024;

    // Need to fork so accept command inside ctor won't stop client creation.
    // This test uses child process to create a server and parent process is the client.
    // Test will be for the data the client receives from the server.
    int pid = fork();
    if (pid == 0) {
        // Child (client) process
        // Wait for server to turn on.
        sleep(1);
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
        // Wait for server to Write the client message
        sleep(1);
        // Recieve data
        char buffer[1024];
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            perror("error receiving data");
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received data

        // Send the data back to the server UPPER CASE
        string upper_case(buffer);
        transform(upper_case.begin(), upper_case.end(), upper_case.begin(), ::toupper);
        int bytes_sent = send(client_sock, upper_case.c_str(), upper_case.size(), 0);
        if (bytes_sent < 0) {
            perror("error sending data");
        }
        // Close client socket
        close(client_sock);
        exit(0); // Exit child process
    } else {
        // Parent (server) process
        // Create server socket with object
        SocketIO serv_socket(port);
        serv_socket.acceptClient();
        // Wait for client to connect
        sleep(1);

        // Write to client
        string text = "Hello from server!";
        serv_socket.write(text);

        // Set buffer to receive data
        string buffer;
        serv_socket.getline(buffer);

        // Check if the received data matches the expected data
        EXPECT_EQ(buffer, "HELLO FROM SERVER!");

        // Socket destructor will close the server socket
        // Wait for child process to finish
        waitpid(pid, nullptr, 0);
    }
}

// Test SocketIO GetLine and Write after
TEST(SocketIO, TestSocketIOGetLineAndWrite) {
    // Generate random port between 1024 and 49151
    int port = rand() % (49151 - 1024 + 1) + 1024;

    // Need to fork so accept command inside ctor won't stop client creation.
    // This test uses child process to create a server and parent process is the client.
    // Test will be for the data the client receives from the server.
    int pid = fork();
    if (pid == 0) {
        // Child (server) process
        // Create server socket
        SocketIO serv_socket(port);
        serv_socket.acceptClient();
        // Wait for client to connect
        sleep(1);
        // GetLine from the client
        string buffer;
        serv_socket.getline(buffer);
        EXPECT_EQ(buffer, "Hello from client!");

        // Write back upper case
        transform(buffer.begin(), buffer.end(), buffer.begin(), ::toupper);
        serv_socket.write(buffer);
        
        // Close server socket is by Destructor, no needed to close.
        exit(0); // Exit child process
    } else {
        // Parent (client) process
        // Wait for server to turn on.
        sleep(1);
        // Create client socket
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
        // Send data to server
        const char* buffer = "Hello from client!";
        int bytes_sent = send(client_sock, buffer, strlen(buffer), 0);
        if (bytes_sent < 0) {
            perror("error sending data");
        }

        // Receive data
        char buffer2[1024];
        int bytes_received = recv(client_sock, buffer2, sizeof(buffer2), 0);
        if (bytes_received < 0) {
            perror("error receiving data");
        }
        buffer2[bytes_received] = '\0'; // Null-terminate the received data

        // Check if the received data matches the expected data
        EXPECT_EQ(string(buffer2), "HELLO FROM CLIENT!");
        // Close client socket
        close(client_sock);

        // Wait for child process to finish
        waitpid(pid, nullptr, 0);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}