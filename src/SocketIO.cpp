#include "IIO.h"
#include "SocketIO.h"
#include <iostream>
#include <cstring>
#include <unistd.h> // For close()
#include <arpa/inet.h> // For inet_pton()
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <string>
#include <sstream> // For stringstream

using namespace std;

namespace Socket {
// Constructor for SocketIO class
SocketIO::SocketIO(int port):port(port), client_socket(-1) {
    // Initialize the socket
    server_socket = createSocket();
    // Set up the server address structure and bind the socket
    bindSocket(port);
    // listen for incoming connections
    // Set the socket to listen for incoming connections
    listenSocket();
    // Accept a client connection
    //acceptClient();
}

// Function to create a socket
int SocketIO::createSocket() {
    // Initialize the socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket failed");
        exit(1);
    }
    return sock;
}

// Function to bind the socket to a port
void SocketIO::bindSocket(int port) {
    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the structure
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any address (
    server_addr.sin_port = htons(port); // Port number in network byte order

    // Bind the socket to the address and port and check that it was successful
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("error binding socket");
        exit(EXIT_FAILURE);
    }
}

// Function to listen for incoming connections
void SocketIO::listenSocket() {
    if (listen(server_socket, 5) < 0) {
        perror("listen failed");
        if (server_socket != -1) ::close(server_socket);
        exit(1);
    }
}

/// Accept a client connection
void SocketIO::acceptClient() { 
    socklen_t client_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
    if (client_socket < 0) {
        perror("accept failed");
        if (server_socket != -1) ::close(server_socket);
        exit(1);
    }
}

// Destructor for SocketIO class
SocketIO::~SocketIO() {
   close(); // Close the sockets
}

// Function to open the socket (not used in this implementation)
int SocketIO::open() {
    // Socket is already opened in the constructor
    return 1; // Success
}


// Function to read a line from the socket 
// This function reads data from the socket and stores it in the provided buffer. It returns 1 on success, 0 on error.
int SocketIO::getline(std::string& buffer) {
    //acceptClient(); - old implementation each getline creates new connection
    // Temporary buffer to read data from the socket
    char temp[1024] = {0};
    // Read data from the socket into the temporary buffer
    int bytes = read(client_socket, temp, sizeof(temp));
    if (bytes <= 0) return 0; // Client disconnected or error occurred
    buffer = std::string(temp, bytes); // Convert to string
    return 1;
}

// Function to close the socket if it is open
int SocketIO::close() {
    if (client_socket != -1) ::close(client_socket);
    if (server_socket != -1) ::close(server_socket);
    return 0;
}

// Function to write data to the socket
// This function sends the provided buffer to the socket. It returns 1 on success, 0 on error.
int SocketIO::write(const std::string& buffer) {
    //acceptClient();
    std::string message = buffer;
    /* At the moment no need to \n terminate a message
    if (message.empty() || message.back() != '\n') {
        message += '\n'; // Append newline character if not present
    }
    */
    int bytes = send(client_socket, message.c_str(), message.size(), 0);
    return bytes > 0 ? 1 : 0;
}

bool SocketIO::eof() const {
    return false;
}

bool SocketIO::deleteline(string buffer) {
    // Deleting lines from a socket is not applicable, so we do nothing
    return true;
}
} // namespace SocketIO