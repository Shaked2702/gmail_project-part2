// Note: All interface classes should recieve another execute argument of IStream.
#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include <iostream>
#include <fstream>
#include "IDataStructure.h"
#include "StreamIO.h"
#include "IIO.h"

using namespace std;
namespace Command {

    class ICommand {
        public:
            virtual ~ICommand() = default;
            virtual int execute(IDataStructure& ids, IIO* url_file_io, istream& input_stream) = 0;
    };
}

#endif// ICOMMAND_H

/* ICommand Execute Integer Mapping:

Add Cases:
0 - Non valid URL -> I'm pretty sure that 400 Bad Request\n (but maybe 404 Not Found\n, maybe ask in forum)
1 - Legal command, URL added & *WAS NOT* in the data structure before. -> 201 Created\n
2 - Legal command, URL added & *ALREADY WAS* in the data structure before. -> 201 Created\n

Check Cases:
0 - Non valid URL -> I'm pretty sure that 400 Bad Request\n (but maybe 404 Not Found\n, maybe ask in forum)
3 - Legal command, URL check is False (not exists) -> 200 Ok\n\nfalse\n
4 - Legal command, URL check is True (exists) -> 200 Ok\n\ntrue true\n
5 - Legal command, URL check is False Positive (not exists) -> 200 Ok\n\ntrue false\n
(6 - Legal command, URL check is False Negative (exists) -> 200 Ok\n\nfalse true\n) Not in bloom filter.
Delete Cases:

0 - Non valid URL -> I'm pretty sure that 400 Bad Request\n (but maybe 404 Not Found\n, maybe ask in forum)
7 - Legal command, URL deleted & *WAS* in ids -> 204 No Content\n
8 - Legal command, Logic problem, URL *NOT EXISTS* in ids -> 404 Not Found\n
*/