#ifndef IDATASTRUCTURE_H
#define IDATASTRUCTURE_H
#include <string>
using namespace std;

    class IDataStructure
    {
        public:
            virtual bool AddUrl(const string& url)=0;
            // 0= false, 1=true true, 2=true false 3= false true
            virtual int CheckUrl(const string& url)=0;
            virtual bool DeleteUrl(const string& url)=0; // Delete URL from the data structure
            virtual ~IDataStructure()=default; // Virtual destructor for proper cleanup of derived classes   
    };
    
#endif //IDataStructure.h