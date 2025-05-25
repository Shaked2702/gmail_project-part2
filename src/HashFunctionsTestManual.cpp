#include "HashFunctions.h"
#include <functional>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    hash<string> hasher;
    cout << HashFunctions::HashFunctions(hasher,1).myHash("www") << ", " << HashFunctions::HashFunctions(hasher,2).myHash("www") << " Should be NOT EQUAL" << endl;
    cout << HashFunctions::HashFunctions(hasher,2).myHash("www") << ", " << HashFunctions::HashFunctions(hasher,3).myHash("www") << " Should be NOT EQUAL" << endl;
    cout << HashFunctions::HashFunctions(hasher,1).myHash("wwx") << ", " << HashFunctions::HashFunctions(hasher,1).myHash("www") << " Should be NOT EQUAL" << endl;
    cout << HashFunctions::HashFunctions(hasher,2).myHash("www") << ", " << HashFunctions::HashFunctions(2).myHash("www") << " Should be NOT EQUAL" << endl;
    cout << HashFunctions::HashFunctions(hasher,1).myHash("www") << ", " << HashFunctions::HashFunctions(hasher,1).myHash("www") << " Should be EQUAL" << endl;
    cout << HashFunctions::HashFunctions(hasher,2).myHash("www") << ", " << HashFunctions::HashFunctions(hasher,2).myHash("www") << " Should be EQUAL" << endl;
    cout << HashFunctions::HashFunctions(hasher,2).myHash("www") << ", " << HashFunctions::HashFunctions(hasher,1).myHash(to_string(HashFunctions::HashFunctions(hasher,1).myHash("www"))) << " Should be EQUAL" << endl;
        
}
/*    
    EXPECT_NE(HashFunctions::HashFunctions(hasher,1).myHash("www"), 
    HashFunctions::HashFunctions(hasher,2).myHash("www"))
        << "Hash values should differ with different iterations";
    EXPECT_NE(HashFunctions::HashFunctions(hasher,2).myHash("www"), 
    HashFunctions::HashFunctions(hasher,3).myHash("www"))
        << "Hash values should differ with different iterations";
    EXPECT_NE(HashFunctions::HashFunctions(hasher,1).myHash("wwx"), 
    HashFunctions::HashFunctions(hasher,1).myHash("www"))
        << "Hash values should differ with different inputs";
    EXPECT_NE(HashFunctions::HashFunctions(hasher,2).myHash("www"), 
    HashFunctions::HashFunctions(2).myHash("www"))
        << "Different hash function instances should retrieve different hash values";
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,1).myHash("www"), 
    HashFunctions::HashFunctions(hasher,1).myHash("www"))
        << "Hash values should be the same for the same input and iterations";
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,2).myHash("www"), 
    HashFunctions::HashFunctions(hasher,2).myHash("www"))
        << "Hash values should be the same for the same input and iterations";
    EXPECT_EQ(HashFunctions::HashFunctions(hasher,2).myHash("www"),
        HashFunctions::HashFunctions(hasher,1).myHash(to_string(HashFunctions::HashFunctions(hasher,1).myHash("www"))))
        << "Hash values should be the same for hashing manually and iteratively";
*/