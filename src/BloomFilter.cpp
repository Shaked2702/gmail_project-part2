#include "StreamIO.h"
#include "IIO.h"
#include "HashFunctions.h"
#include "BloomFilter.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
using std::vector;
using std::string;
using namespace std;

   BloomFilter::BloomFilter(vector<int> iter_hash,size_t array_size): 
   m_urlsArray(0), // Number of urls is 0 in the beginning
   m_dirtyBit(array_size, 0) // Dirty bits array with given size, initialize to 0
   {
       for(int i = 0; i < iter_hash.size(); i++) {
           m_hashFunctions.push_back(HashFunctions::HashFunctions(iter_hash[i])); // Initialize hash functions with given iter
        }
    }


   BloomFilter::~BloomFilter() {
    //    clear(m_urlsArray) // Clear the urls array
    //    clear(m_dirtyBit) // Clear the dirty bits array
    //    clear(m_hashFunctions) // Clear the hash functions array
    }

    // Add a URL to the Bloom filter. Return true iff added successfully.
    bool BloomFilter::AddUrl(const string& url){
        if (CheckUrl(url) == 1) {
           return false; // URL already exists, no need to add
        }
        // If URL is not found, add it to the array
        m_urlsArray.push_back(url);
        // Add to dirty bit array
        for (const auto& hashFunction : m_hashFunctions) {
            size_t number = hashFunction.myHash(url);
            number = number % m_dirtyBit.size();
            m_dirtyBit[number] = 1; // Set the bit to 1
        }
        return true; // URL added successfully
    }

    // Return value: 0=false, 1=true true, 2=true false, 3=false true
    int BloomFilter::CheckUrl(const string& url) {
        // Check if the URL is in the array
        for (const auto& hashFunction : m_hashFunctions) {
            size_t number = hashFunction.myHash(url);
            number = number % m_dirtyBit.size();
            if (m_dirtyBit[number] == 0){
               return 0; // URL not found
            }
        }
        if(FalsePositive(url)){
            return 1; // URL found in bits and also in URL array - true positive.
        }
        return 2; // URL bits are found but do not exist - false positive.
    }

   bool BloomFilter::FalsePositive(const string& url) {
        // Check if the URL is in the list of URLs
        auto it = find(m_urlsArray.begin(), m_urlsArray.end(), url);

        // Check if element was found
        if (it != m_urlsArray.end()) {
            return true; // URL found
        } else {
            return false; // URL not found
        }
    }
    bool BloomFilter::DeleteUrl(const string& url) {
        // Check if the URL is in the array
        auto it = find(m_urlsArray.begin(), m_urlsArray.end(), url);
        if (it != m_urlsArray.end()) {
            m_urlsArray.erase(it); // Remove the URL from the array
            return true; // URL deleted successfully
        }
        return false; // URL not found, nothing to delete
    }
    // Getter for hash functions
    vector<HashFunctions::HashFunctions> const BloomFilter::getHashFunctions() {
        return m_hashFunctions;
    } 