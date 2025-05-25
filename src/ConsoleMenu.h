#ifndef CONSOLEMENU_H
#define CONSOLEMENU_H
#include <map>
#include "IMenu.h"
#include "StreamIO.h"
#include "IIO.h"
#include <string>
#include <iostream>
#include <fstream>
#define OPTIONS 2 
    class ConsoleMenu : public IMenu {
        private:
        //initializing the command map with the command numbers.
        map<string,int> m_command_number= {
            {"POST", 1},
            {"GET", 2},
            {"DELETE", 3},
        };
        bool checkFile();
        bool CheckInput(string& UserInput);
        public:
        int GetCommand(IIO* input, ostream& output) override;
    };
#endif // CHECKCOMMAND_H