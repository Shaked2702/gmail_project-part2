#ifndef LINESETUP_H
#define LINESETUP_H

#include "ISetup.h"
#include "IDataStructure.h"
#include <vector>
#include <string>
#include <iostream>
#include "StreamIO.h"
#include "IIO.h"
namespace Setup {
    class LineSetup : public ISetup {     
public:
// Implements the Isetup interface to initialize the data structure.
IDataStructure* InitializeIDS(const std::string& setupFile, const std::string& urlFile, istream& input_stream) override ;

// Check if the file exists.
bool FileExists(const std::string& fileName) const;

private:
 // Check if the string is all numbers.
bool IsValidNumber(const std::string& str) const;

// Check if the input is valid. 
bool ParseInput(const std::string& input, std::vector<int>& iter_hash, size_t& array_size) const;


// initialize the data structure from the input.
IDataStructure* InitFromInput(const std::string& setupFile, istream& input_stream);

// Add URLs from the file to the data structure.
void addUrlFromFile(IDataStructure& ids, const std::string& urlFile);
};
}
#endif // LINESETUP_H