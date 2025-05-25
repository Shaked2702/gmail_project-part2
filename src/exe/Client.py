
# Import the socket and sys libraries
import socket
import sys

# Check if the correct number of command line arguments are provided
if len(sys.argv) != 3:
    print("Usage: python3 client.py <server_ip> <port>")
    exit(1)

# Get the server IP address from command line arguments
server_ip = sys.argv[1]
# Get the port number from command line arguments
port = int(sys.argv[2])

# Check if the server IP address is valid
try:
    socket.inet_aton(server_ip)
except socket.error:
    print("Invalid server IP address")
    exit(1)

# Check if the port number is valid
if port < 1024 or port > 49151:
    print("Port number must be between 1024 and 49151")
    exit(1)

# Create a TCP socket using IPv4 addressing
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect to the server using the provided IP address and port
s.connect((server_ip, port))

# Get commands from the user and send them to the server in a loop and print the response
try:
    while True:
        command = input()
        s.send((command + '\n').encode())
        response = s.recv(1024).decode()
        print(response.rstrip('\n'))
finally:
    s.close()
