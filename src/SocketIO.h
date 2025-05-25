#ifndef SOCKETIO_H
#define SOCKETIO_H
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;
namespace Socket{
class SocketIO : public IIO {
    private:
    int server_socket; // Server socket descriptor
    int client_socket; // Client socket descriptor
    int port; // Port number
    sockaddr_in server_addr, client_addr; // Server and client address structures
        
     // Function to create a socket
    int createSocket();
        
     // Function to bind the socket to a port
    void bindSocket(int port);

     // Function to listen for incoming connections
    void listenSocket();
 
    public:

        // Constructor
        SocketIO(int port);

        // Destructor
        ~SocketIO() override;

        // Open function - connect to the server
        int open() override;

        // Close function - close the socket
        int close() override;

        // Function to accept a client connection
        void acceptClient();

        // Get line from input stream - return 1 for success, 0 for EOF or error.
        int getline(string& buffer) override;

        // Write to output stream - return 1 for success, 0 for error.
        int write(const string& buffer) override;

        // Check if end of file is reached.
        bool eof() const override;

        // Delete line from stream - return 1 for success, 0 for error. Does nothing for sockets.
        bool deleteline(string buffer) override;
};
}
#endif // SOCKETIO_H