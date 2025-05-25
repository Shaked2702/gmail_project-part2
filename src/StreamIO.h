#ifndef STREAMIO_H
#define STREAMIO_H

#include <iostream>
#include "IIO.h"

using namespace std;

class StreamIO : public IIO {
    private:
        // Note: fstream extends istream and ostream, thus we can use same stream for both fields.
        istream* m_in_stream; // Input stream
        ostream* m_out_stream; // Output stream
        const string m_file_path; // File path - use in can we want to reopen a file.
        int m_state; // Last state of the stream (in files only)
        // -1 - neutral 0 - reading 1 - writing

        // Null string "" for console input/output.

    public: 
        // Ctor
        StreamIO(const string& file_path);

        // Destructor
        ~StreamIO() override;

        // Open function - temporarily empty usage.
        int open();

        // Close function - temporarily empty usage.
        int close();

        // Get line from input stream - return 1 for success, 0 for EOF or error.
        int getline(string& buffer);

        // Write to output stream - return 1 for success, 0 for error.
        // Write always to EOF.
        int write(const string& buffer);
        
        // Check if end of file is reached.
        bool eof() const;

        // Delete line from from stream - return 1 for success, 0 for error. Does nothing for cin/cin.
        bool deleteline(string buffer);
};






#endif // STREAMIO_H