// Interface for Input/Output operations: Stream (File/Console) and Sockets (Maybe Database in future)
#ifndef IIO_H
#define IIO_H

#include <string>
#include <iostream>
#include <fstream>
#include "IDataStructure.h"
using namespace std;

    class IIO {
        public:
            // No constructor - interface
            // Destructor - using virtual to ensure proper polymorphism destruction
            virtual ~IIO() = default;
            
            // No argument because different between classes (Path/ipv4) thus defined in derived classes constructor.
            virtual int open()=0;
            
            // Close function - closes IO object.
            virtual int close()=0;
            
            // Get line from input stream - return 1 for success, 0 for EOF or error.
            virtual int getline(string& buffer)=0;

            // Write to output of IO - return 1 for success, 0 for error.
            virtual int write(const string& buffer)=0;

            // Check if end of file is reached - non relevant IO objects should return always false.
            virtual bool eof() const=0;

            // Delete line from stream - return 1 for success, 0 for error -
            // Non relevant IO objects should return true and do nothing (Socket/Console).
            virtual bool deleteline(string buffer)=0;
    };

#endif// IIO_H