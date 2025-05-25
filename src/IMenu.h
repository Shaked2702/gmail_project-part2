#ifndef IMENU_H
#define IMENU_H

#include <string>
#include "StreamIO.h"
#include "IIO.h"
using namespace std;

    class IMenu
    {
        public:
            //geting 1 or 2 from the user to know which command he wants to do.
            virtual int GetCommand(IIO* input, ostream& output)=0; 
    };
#endif // IMENU_H