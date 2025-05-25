#ifndef ISETUP_H
#define ISETUP_H
#include <string> 
#include <iostream>
#include "StreamIO.h"
#include "IIO.h"

using namespace std;

class IDataStructure; // Forward declaration of IDataStructure class
namespace Setup {
    class ISetup {
        public:
        virtual IDataStructure* InitializeIDS(const std::string& setupFile, const std::string& urlFile, istream& input_stream) = 0;
        virtual ~ISetup() = default; // Virtual destructor for proper cleanup of derived classes
    };
}
#endif // ISETUP_H