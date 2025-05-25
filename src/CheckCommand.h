#ifndef CHECKCOMMAND_H
#define CHECKCOMMAND_H

#include "ICommand.h"
#include "IDataStructure.h"
#include "StreamIO.h"
#include "IIO.h"
#include <iostream>
#include <fstream>
namespace Command {

    class CheckCommand : public ICommand {
        private:
            // Returns empty string if non valid command.
            string isValidUrl(istream& input_stream);
            string ToLowerCase(string& url);
        public:
            CheckCommand();
            virtual ~CheckCommand();
            int execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream);
    };
}

#endif // CHECKCOMMAND_H