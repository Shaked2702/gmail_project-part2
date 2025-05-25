#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "ICommand.h"
#include "IDataStructure.h"
#include "StreamIO.h"
#include "IIO.h"
#include <string>
#include <iostream>
#include <fstream>

namespace Command {
    class AddCommand : public ICommand {
        private:
            // Returns empty string if non valid command.
            string isValidUrl(istream& input_stream);
            string ToLowerCase(string& url);
        public:
            // Constructor
            AddCommand();
            // Destructor
            virtual ~AddCommand();
            //execute function to add or check URL to the data structure.
            int execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream);
    };
}

#endif // ADDCOMMAND_H