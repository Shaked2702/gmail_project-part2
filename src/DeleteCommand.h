#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include "ICommand.h"
#include "IDataStructure.h"
#include "StreamIO.h"
#include "IIO.h"
#include <iostream>
#include <fstream>
namespace Command {

    class DeleteCommand : public ICommand {
        private:
            // Returns empty string if non valid command.
            string isValidUrl(istream& input_stream);
            string ToLowerCase(string& url);
            // void FileDeleteUrl(string& url); // Redundant - IIO object deletes line from file.
        public:
            DeleteCommand();
            virtual ~DeleteCommand();
            int execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream);
    };
}
#endif // DELETECOMMAND_H