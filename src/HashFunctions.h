#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H
#include <functional>
#include <string>

using namespace std;
namespace HashFunctions
{
    class HashFunctions
    {
        private:
            hash<string> m_hasher;
            int m_iter;

        public:
            HashFunctions(hash<string> hasher, int iter); // Usage constructor for tests file
            HashFunctions(int iter); // Usage constructor for progream.
            ~HashFunctions(); // Destructor.
            size_t myHash(string url) const;       
    };
}
#endif// HashFunctions.h