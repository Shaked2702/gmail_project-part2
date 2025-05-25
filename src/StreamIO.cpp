// Important behavior of StreamIO - Not usable to read and write at the same time.
#include "StreamIO.h"
#include "IIO.h"
#include <vector>

using namespace std;

// Ctor
StreamIO::StreamIO(const string& file_path)
    : m_in_stream(&cin), m_out_stream(&cout), m_file_path(file_path), m_state(-1) {
        // Null string "" define for cin cout
        if (file_path != "") {
            // Flags explanation - append affect only write to EOF, read still start from beginning
            auto file_stream = new fstream(file_path, ios::in | ios::out | ios::app);
            // Auto is pointer to fstream - istream and ostream are both extended by fstream
            m_in_stream = file_stream;
            m_out_stream = file_stream;
        }
}

// Dtor - notice no need to make close() before using destructor - if making no bug occur.
StreamIO::~StreamIO() {
    if (m_file_path != "") {
        close();
        // Same allocated object for both - no need delete out also.
    }
    // Cout cin no need to delete.
}

// Open function - empty usage for now.
int StreamIO::open() {
    if (m_file_path != "") {
        if (m_in_stream == nullptr) { // Safety if stream is closed only open it
            auto file_stream = new fstream(m_file_path, ios::in | ios::out | ios::app);
            // Auto is pointer to fstream - istream and ostream are both extended by fstream
            m_in_stream = file_stream;
            m_out_stream = file_stream;
            m_state = -1; // Position could be also start for read and also EOF for write.
            return 1;    
        }
        return 0; // Stream already opened
    }
    return 1;
}

// Close function - empty usage for now.
int StreamIO::close() {
    if (m_file_path != "") {
        if (m_in_stream == nullptr) { // Safety if stream is closed no close needed.
            return 0; // Stream already closed
        }
        // Cast to fstream to close the file stream
        fstream* file_stream = dynamic_cast<fstream*>(m_in_stream);
        if (file_stream) {
            file_stream->close(); // Close the file stream
        }
        delete m_in_stream;
        m_in_stream = nullptr;
        m_out_stream = nullptr;
        m_state = -1; // Set state to neutral
    }
    return 1;
}

// Get line from input stream
int StreamIO::getline(string& buffer) {
    
    // Cin block and exit
    if (m_file_path == "") {
        if (std::getline(*m_in_stream, buffer)) {
            return 1; // Success
        }
        return 0; // EOF or error
    }

    // By definition stream is nullpter iff stream is closed.
    if (m_in_stream == nullptr) {
        open();
    }

    // If last operation was WRITE the pointer is at EOF so must move to the beginning.
    if (m_state != 0) {
        m_in_stream->seekg(0, ios::beg); // Move to the beginning of the file
        m_state = 0; // Set state to reading
    }

    // EOF return 0
    if (m_in_stream->eof()) {
        return 0; // End of file
    }

    // Set input into buffer
    // getline returns stream this we can check if eof
    if (std::getline(*m_in_stream, buffer).eof()) {
        return 0; // End of file
    }

    // Check if stream is in error state
    if (m_in_stream->fail()) {
        return 0; // Error
    }
    return 1; // Success
    
}

// Write to output stream - notice opened in append mode always
int StreamIO::write(const string& buffer) {
    // Cout block - no \n terminated automatically
    if (m_file_path == "") {
        *m_out_stream << buffer; // Write to cout
        return 1; // Success
    }


    // nullptr in stream means closed stream.
    if (m_out_stream == nullptr) {
        open();
    }
    // Set pointer to the end of the file
    m_out_stream->seekp(0, ios::end); // Move to the end of the file

    // Set last state to writing
    m_state = 1; // Set state to writing

    // Write by appending to eof - not include \n
    *m_out_stream << buffer;

    if (m_out_stream->fail()) {
        return 0; // Error
    }
    return 1; // Success
}

// Check if stream in end of file.
bool StreamIO::eof() const {
    if (m_in_stream == nullptr) {
        return true; // Stream is closed
    }
    return m_in_stream->eof();
}

// Delete line from stream - include it's \n. For cin/cout does nothing.
// Must set seek pointer to EOF at finish.
// Buffer should include \n inside
bool StreamIO::deleteline(string buffer) {
    if (m_file_path == "") {
        return false; // No delete is allowed from cin/cout
    }
    // nullptr in stream means closed stream.
    if (m_in_stream == nullptr) {
        open();
    }
    // Find the exact position of the "buffer string" in the fstream
    auto file_stream = dynamic_cast<fstream*>(m_in_stream);
    // Move pointer to the beginning of the file
    file_stream->seekg(0, ios::beg);


    // Remove \n from end of buffer - string isn't reference but a copy so modification is allowed.
    if (buffer.back() == '\n') {
        buffer.pop_back(); // Remove the last character
    }

    vector<string> lines; // Vector to store lines
    string line;
    // Read the file and store lines in vector without the matching line
    while (std::getline(*file_stream, line)) {
        if (line != buffer) {
            lines.push_back(line); // Store the line if it doesn't match
        }
    }
    
    // Close stream and remove file
    this->close();
    remove(m_file_path.c_str()); // Remove the file
    
    // Open new stream (creates new file) - manually because it's a new create
    auto file_stream2 = new fstream(m_file_path, ios::out | ios::trunc);
    if (!file_stream2->is_open()) {
        return false;
    } // Yet not saving that stream he is trunc mode (no apped)
    
    // Write the remaining lines to the file with \n
    for (const auto& l : lines) {
        *file_stream2 << l << "\n"; // Append each line with \n
    }

    file_stream2->flush(); // Ensure all data is written to the file
    file_stream2->close(); // Close the file to ensure consistency

    delete file_stream2;

    this->open(); // Reopen the stream to reset the state
    m_state = -1; // Like neutral (new file)

    return true; // Success
}