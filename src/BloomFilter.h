
#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include <iostream>
#include "IDataStructure.h"
#include "HashFunctions.h"
#include "StreamIO.h"
#include "IIO.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
class BloomFilter : public IDataStructure{
   private:
      vector<string> m_urlsArray; // Stores URLs
      vector<char> m_dirtyBit;         // Stores dirty bits
      vector<HashFunctions::HashFunctions> m_hashFunctions; // Stores hash function
      bool FalsePositive(const string& url);

    public:
      BloomFilter(vector<int> iter_hash,size_t array_size); 
      virtual bool AddUrl(const string& url) override;
      virtual int CheckUrl(const string& url) override;
      virtual ~BloomFilter() override;
      virtual bool DeleteUrl(const string& url) override;
      vector<HashFunctions::HashFunctions> const getHashFunctions(); // Getter for hash functions
};
#endif //BloomFilter.h