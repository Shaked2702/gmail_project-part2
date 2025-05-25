#include <gtest/gtest.h>
#include "HashFunctions.h"
#include "BloomFilter.h"
#include "IDataStructure.h"
#include "ICommand.h"
#include "AddCommand.h"
#include "CheckCommand.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;
using namespace Command;

/*------------------------------------HashFunctionSuite-----------------------------------------*/

// Test Constructor and 2 hash functions with different iterations different values.
TEST(HashFunctions, ConstructorSanityAndDifferentIterations) {
    hash<string> hasher;
    EXPECT_NE(HashFunctions::HashFunctions(hasher,1).myHash("www"), 
    HashFunctions::HashFunctions(hasher,2).myHash("www"))
        << "Hash values should differ with different iterations";
}

// Test different iterations between same hash different values - case of more than 1 iteration.
TEST(HashFunctions, DifferentIterationsMoreThanOne) {
    hash<string> hasher;
    EXPECT_NE(HashFunctions::HashFunctions(hasher,2).myHash("www"), 
    HashFunctions::HashFunctions(hasher,3).myHash("www"))
        << "Hash values should differ with different iterations";
}

// Test same hash for different string inputs - value should be different.
TEST(HashFunctions, DifferentInputString) {
    hash<string> hasher;
    EXPECT_NE(HashFunctions::HashFunctions(hasher,1).myHash("wwx"), 
    HashFunctions::HashFunctions(hasher,1).myHash("www"))
        << "Hash values should differ with different inputs";
}

// Test each creation generates same hash values - Meaning deterministic hash function.
TEST(HashFunctions, DifferentInstancesOfHashFunctions) {
    hash<string> hasher;
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,2).myHash("www"), 
    HashFunctions::HashFunctions(2).myHash("www"))
        << "Different hash function instances should retrieve equal hash values";
}

// Test equal value for same iterations and base hash for same string.
TEST(HashFunctions, EqualSameStringSameIterations) {
    hash<string> hasher;
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,1).myHash("www"), 
    HashFunctions::HashFunctions(hasher,1).myHash("www"))
        << "Hash values should be the same for the same input and iterations";
}

// Test equal value for same iterations and base hash for same string - more than 1 iteration.
TEST(HashFunctions, EqualSameStringSameMultipleIterations) {
    hash<string> hasher;
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,2).myHash("www"), 
    HashFunctions::HashFunctions(hasher,2).myHash("www"))
        << "Hash values should be the same for the same input and iterations";
}

// Test the recursion of making hash "k" times - the manual output and the object should be the same.
TEST(HashFunctions, RecursiveHashingTest) {
    hash<string> hasher;
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,2).myHash("www"),
        HashFunctions::HashFunctions(hasher,1).myHash(to_string(HashFunctions::HashFunctions(hasher,1).myHash("www"))))
        << "Hash values should be the same for hashing manually and iteratively";
}

/*-------------------------------IDataStructureSuite-----------------------------------------*/

//TEST FALSE AND TRUE POSITIVE;
   TEST(IDataStructure, BloomFilter) {
    vector<int> iter_hash = {1, 2, 3}; // Example hash function iterations
    BloomFilter BloomFilter(iter_hash, 20);
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 0) << "URL should not be found in the empty Bloom filter";
    BloomFilter.AddUrl("www"); // Add a URL to the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 1) << "URL should be found in the Bloom filter after adding it";
    EXPECT_EQ(BloomFilter.CheckUrl("wwx"), 0) << "URL should not be found in the Bloom filter";
    // It could be return value 2 false-positive (but low probability).
    BloomFilter.AddUrl("wwx"); // Add another URL to the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("wwx"), 1) << "URL should be found in the Bloom filter after adding it";
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 1) << "URL should be found in the Bloom filter after adding it";
    BloomFilter.DeleteUrl("wwx"); // Delete the URL from the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("wwx"), 2) << "URL should not be found in the Bloom filter after deleting it";
    BloomFilter.DeleteUrl("www"); // Delete the URL from the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 2) << "URL should not be found in the Bloom filter after deleting it but false positive";
    BloomFilter.DeleteUrl("www"); // Delete the URL from the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 2) << "URL should not be found in the Bloom filter after deleting it";
}
//TEST FALSE NEGATIVE 
TEST(IDataStructure, BloomFilter2) {
    vector<int> iter_hash = {1, 2}; // Example hash function iterations
    BloomFilter BloomFilter(iter_hash, 1);
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 0) << "URL should not be found in the empty Bloom filter";
    BloomFilter.AddUrl("www"); // Add a URL to the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 1) << "URL should be found in the Bloom filter after adding it";
    EXPECT_EQ(BloomFilter.CheckUrl("wwx"), 2) << "URL should not be found in the Bloom filter, but false positive, one bit only";
}

//TEST BLOOM FILTER AND HASH FUNCTIONS CLASSES
TEST(IDataStructure, BloomFilterHashFunctions) {
    vector<int> iter_hash = {1, 2}; // Example hash function iterations
    BloomFilter BloomFilter(iter_hash, 1);
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 0) << "URL should not be found in the empty Bloom filter";
    BloomFilter.AddUrl("www"); // Add a URL to the Bloom filter
    EXPECT_EQ(BloomFilter.getHashFunctions()[0].myHash("www"),
        BloomFilter.getHashFunctions()[0].myHash("www")) << "Hash values should be the same for the same input and iterations";
    EXPECT_EQ(BloomFilter.getHashFunctions()[1].myHash("www"),
        BloomFilter.getHashFunctions()[1].myHash("www")) << "Hash values should be the same for the same input and iterations";
    EXPECT_NE(BloomFilter.getHashFunctions()[1].myHash("www"),
        BloomFilter.getHashFunctions()[1].myHash("wwx")) << "Hash values should not be the same for the same input and iterations";

}

//TEST BLOOMFILTER FULL CAPACITY (bits^hashes - amount of combinations)
TEST(IDataStructure, BloomFilterFullCapacity) {
    vector<int> iter_hash = {1, 2};
    BloomFilter BloomFilter(iter_hash, 4); // 4^2 = 16 combinations
    BloomFilter.AddUrl("www.example1.com");
    BloomFilter.AddUrl("www.example2.com");
    BloomFilter.AddUrl("www.example3.com");
    BloomFilter.AddUrl("www.example4.com");
    BloomFilter.AddUrl("www.example5.com");
    BloomFilter.AddUrl("www.example6.com");
    BloomFilter.AddUrl("www.example7.com");
    BloomFilter.AddUrl("www.example8.com");
    BloomFilter.AddUrl("www.example9.com");
    BloomFilter.AddUrl("www.example10.com");
    BloomFilter.AddUrl("www.example11.com");
    BloomFilter.AddUrl("www.example12.com");
    BloomFilter.AddUrl("www.example13.com");
    BloomFilter.AddUrl("www.example14.com");
    BloomFilter.AddUrl("www.example15.com");
    BloomFilter.AddUrl("www.example16.com");
    EXPECT_EQ(BloomFilter.CheckUrl("www.example17.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.aaa.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.bbb.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.ccc.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.ddd.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.eee.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.fff.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.ggg.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.hhh.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.iii.com"), 2) << "Should be false positive - full database.";
}
//TEST BLOOMFILTER FULL CAPACITY (bits^hashes - amount of combinations)
TEST(IDataStructure, BloomFilterFullCapacityDelete) {
    vector<int> iter_hash = {1, 2};
    BloomFilter BloomFilter(iter_hash, 4); // 4^2 = 16 combinations
    BloomFilter.AddUrl("www.example1.com");
    BloomFilter.AddUrl("www.example2.com");
    BloomFilter.AddUrl("www.example3.com");
    BloomFilter.AddUrl("www.example4.com");
    BloomFilter.AddUrl("www.example5.com");
    BloomFilter.AddUrl("www.example6.com");
    BloomFilter.AddUrl("www.example7.com");
    BloomFilter.AddUrl("www.example8.com");
    BloomFilter.AddUrl("www.example9.com");
    BloomFilter.AddUrl("www.example10.com");
    BloomFilter.AddUrl("www.example11.com");
    BloomFilter.AddUrl("www.example12.com");
    BloomFilter.AddUrl("www.example13.com");
    BloomFilter.AddUrl("www.example14.com");
    BloomFilter.AddUrl("www.example15.com");
    BloomFilter.AddUrl("www.example16.com");
    BloomFilter.DeleteUrl("www.example1.com");
    BloomFilter.DeleteUrl("www.example2.com");
    BloomFilter.DeleteUrl("www.example3.com");
    BloomFilter.DeleteUrl("www.example4.com");
    BloomFilter.DeleteUrl("www.example5.com");
    BloomFilter.DeleteUrl("www.example6.com");
    BloomFilter.DeleteUrl("www.example7.com");
    BloomFilter.DeleteUrl("www.example8.com");
    BloomFilter.DeleteUrl("www.example9.com");
    BloomFilter.DeleteUrl("www.example10.com");
    BloomFilter.DeleteUrl("www.example11.com");
    BloomFilter.DeleteUrl("www.example12.com");
    BloomFilter.DeleteUrl("www.example13.com");
    BloomFilter.DeleteUrl("www.example14.com");
    BloomFilter.DeleteUrl("www.example15.com");
    BloomFilter.DeleteUrl("www.example16.com");
    EXPECT_EQ(BloomFilter.CheckUrl("www.example1.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example2.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example3.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example4.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example5.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example6.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example7.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example8.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example9.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example10.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example11.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example12.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example13.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example14.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example15.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.example16.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.aaa.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.bbb.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.ccc.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.ddd.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.eee.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.fff.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.ggg.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.hhh.com"), 2) << "Should be false positive - full database.";
    EXPECT_EQ(BloomFilter.CheckUrl("www.iii.com"), 2) << "Should be false positive - full database.";
}


//TEST CASE OF LONG URL
TEST(IDataStructure, BloomFilterLongUrl) {
    vector<int> iter_hash = {3, 4};
    BloomFilter BloomFilter(iter_hash, 20);
    BloomFilter.AddUrl("www.example/thisisaverylongurlexampleexampleurliwantitttobesolonggggggggggggggggggggggggggggggggggggggggggffsssssssssssssssssssssssssss.com");
    EXPECT_EQ(BloomFilter.CheckUrl("www.example/thisisaverylongurlexampleexampleurliwantitttobesolonggggggggggggggggggggggggggggggggggggggggggffsssssssssssssssssssssssssss.com"),1);
    // Long Url should return 1 - true positive.
}

//TEST CASE OF ZERO HASHES - SHOULD USE DIRRECTLY THE URL LIST - FALSE POSITIVE
TEST(IDataStructure, BloomZeroHashes) {
    vector<int> iter_hash = {0};
    BloomFilter BloomFilter(iter_hash, 1);
    BloomFilter.AddUrl("www.aaa.com");
    EXPECT_EQ(BloomFilter.CheckUrl("www.aaa.com"), 1) << "URL should be found in the Bloom filter after adding it";
    EXPECT_EQ(BloomFilter.CheckUrl("www.bbb.com"), 2) << "URL should not be found in the Bloom filter, false positive because of 0 hashes";
    BloomFilter.DeleteUrl("www.aaa.com"); // Delete the URL from the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("www.aaa.com"), 2) << "URL should not be found in the Bloom filter after deleting it so false positive because of 0 hashes";
    // "Emptyly true because of 0 hashes".
}

//TEST CASE OF EMPTY BLOOM FILTER
//Should not allow, excpetion will occur for operation modolu 0 (%0).
/*TEST(IDataStructure, BloomZeroBitArray) {
    vector<int> iter_hash = {1, 2};
    BloomFilter BloomFilter(iter_hash, 0);
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 0) << "URL should not be found in the empty Bloom filter";
    BloomFilter.AddUrl("www"); // Add a URL to the Bloom filter
    EXPECT_EQ(BloomFilter.CheckUrl("www"), 1) << "URL should be found in the Bloom filter after adding it";
    EXPECT_EQ(BloomFilter.CheckUrl("wwx"), 0) << "URL should not be found in the Bloom filter";
}*/ 

//TEST PREFORMANCE OF IDataStructure FOR BIG AMOUNT OF DATA
TEST(IDataStructure, BloomPerformanceTest) {
    vector<int> iter_hash = {5, 2, 8};
    BloomFilter BloomFilter(iter_hash, 100000);
    // Delete a large number of URLs which are not exist in the filter
    for (int i = 0; i < 10000; i++) {
       EXPECT_EQ(BloomFilter.DeleteUrl("www.example" + to_string(i) + ".com"),0)
       << "URL should not be found in the Bloom filter, false";
    }
    // Add a large number of URLs
    for (int i = 0; i < 10000; i++) {
        BloomFilter.AddUrl("www.example" + to_string(i) + ".com");
    }

    // Check a large number of URLs
    for (int i = 0; i < 10000; i++) {
            EXPECT_EQ(BloomFilter.CheckUrl("www.example" + to_string(i) + ".com"), 1)
            << "URL should be found in the Bloom filter";
    }
    // Delete a large number of URLs
    for (int i = 0; i < 10000; i++) {
           EXPECT_EQ(BloomFilter.DeleteUrl("www.example" + to_string(i) + ".com"), 1)
           << "URL should be found in the Bloom filter";
    }
    // Check a large number of URLs
    for (int i = 0; i < 10000; i++) {
            EXPECT_EQ(BloomFilter.CheckUrl("www.example" + to_string(i) + ".com"), 2)
            << "URL should not be found in the Bloom filter,false positive";
    }
}


/*-----------------------------------------------------------------------------------------------------*/
/*---NEW SUITE TESTS---BloomFilter------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}