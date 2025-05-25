/* HashFunctions assume using int 32bit*/
#include "HashFunctions.h"
#include <random>
#include <iostream>

using namespace std;
namespace HashFunctions{    
    HashFunctions::HashFunctions(int iter) : m_iter(iter), m_hasher(hash<string>()) {}

    // Usage constructor for tests only.
    HashFunctions::HashFunctions(hash<string> hasher, int iter) : 
    m_hasher(hasher), m_iter(iter) {}

    // Destructor.
    HashFunctions::~HashFunctions() {
        // Currently, no dynamic memory allocation is done, so nothing to clean up.
    }

    size_t HashFunctions::myHash(string url) const {
        size_t num = m_hasher(url);
        for (int i = 1; i < m_iter; i++) {
            num = m_hasher(to_string(num));
        }
        return num;
    }
}