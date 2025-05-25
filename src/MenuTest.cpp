// Note: First test is truncating the url_test.txt file.
#include <gtest/gtest.h>
#include "HashFunctions.h"
#include "IDataStructure.h"
#include "ICommand.h"
#include "AddCommand.h"
#include "CheckCommand.h"
#include "DeleteCommand.h"
#include "IMenu.h"
#include "ConsoleMenu.h"
#include "IDataStructure.h"
#include "BloomFilter.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace Command;

// Test input output from terminal
//test currect output without url
TEST(IMenu, CorrectOutputWitoutUrl) {
    ConsoleMenu menu;
    istringstream input("POST \nGET \nDELETE \nGET \nx\n\nGET \n");
    ostringstream output;
    
    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin

    IIO * input_io = new StreamIO(""); // Object stands for cin IO

    EXPECT_EQ(menu.GetCommand(input_io, output), 1);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 3);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 0);
    EXPECT_EQ(menu.GetCommand(input_io, output), 0);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), -1); // Empty input on last one

    delete(input_io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
}

//test correct output with urls.
TEST(IMenu, CorrectOutputWithUrl) {
    ConsoleMenu menu;
    istringstream input("POST https://www.example1.com\nGET https://www.example2.com\nDELETE https://www.bad.com\nGET https://www.bad.com\nGET https://www.example3.com\n\n\n https://www.example");
    ostringstream output;

    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin

    IIO * input_io = new StreamIO(""); // Object stands for cin IO

    EXPECT_EQ(menu.GetCommand(input_io, output), 1);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 3);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 0); // empty input

    delete(input_io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
}

/*
//Test real cin - Test in comment to stop automatic testing from usage.
//Use that input: "1 https://www.example1.com\n2 https://www.example2.com\n3 https://www.bad.com\n1https://www.bad.com\n2 https://www.example3.com\n\n\n https://www.example"
//Note that interactive cin terminal should get CTRL+D to stop console input.
TEST(IMenu, TestRealCin) {
    ConsoleMenu menu;
    ostringstream output;

    IIO * input_io = new StreamIO("");

    EXPECT_EQ(menu.GetCommand(input_io, output), 1);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 0); // empty input
}
*/

//Test case of no space between command and url string.
TEST(IMenu, CheckNoSpace) {
    ConsoleMenu menu;
    istringstream input("POST https://www.example1.com\nDELETE https://www.bad.com\nGET https://www.bad.com\nGEThttps://www.example3.com\n\n\n https://www.example");
    ostringstream output;

    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin

    IIO * input_io = new StreamIO(""); // Object stands for cin IO

    EXPECT_EQ(menu.GetCommand(input_io, output), 1);
    EXPECT_EQ(menu.GetCommand(input_io, output), 3);
    EXPECT_EQ(menu.GetCommand(input_io, output), 2);
    EXPECT_EQ(menu.GetCommand(input_io, output), 0); // empty input
    EXPECT_EQ(menu.GetCommand(input_io, output), 0); // empty input
    EXPECT_EQ(menu.GetCommand(input_io, output), 0); // empty input

    delete(input_io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
}

//Check if the output to the stream is correct.
TEST(IMenu, StreamOutputChecks) {
    ConsoleMenu menu;
    istringstream input("POST https://www.example1.com\nGET https://www.example2.com\nGET  \n");
    ostringstream output;

    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin
    IIO * input_io = new StreamIO(""); // Object stands for cin IO

    menu.GetCommand(input_io, output);
    string expectedOutput = "https://www.example1.com\n";
    EXPECT_EQ(output.str(), expectedOutput);

    output.str(""); // Clear the output stream
    menu.GetCommand(input_io, output);
    expectedOutput = "https://www.example2.com\n";
    EXPECT_EQ(output.str(), expectedOutput);

    output.str(""); // Clear the output stream
    menu.GetCommand(input_io, output);
    expectedOutput = " \n";
    EXPECT_EQ(output.str(), expectedOutput);

    delete(input_io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
}

//Check if the output to the stream is correct.
TEST(IMenu, BigInputChecks) {
    ConsoleMenu menu;
    ostringstream output;
    ostringstream oss;
    for(int i = 0; i < 10000; i++) {
        oss << "POST " << "https://www.example" << i << ".com\n";
    }
    istringstream input(oss.str());

    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin
    IIO * input_io = new StreamIO(""); // Object stands for cin IO

    for(int i = 0; i < 10000; i++) {
        output.str(""); // Clear the output stream
        output.clear(); // Clear the output stream state
        EXPECT_EQ(menu.GetCommand(input_io, output), 1);
        EXPECT_EQ(output.str(), "https://www.example" + to_string(i) + ".com\n");
    }

    delete(input_io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
}
//Check leading spaces in the input stream.
TEST(IMenu, LeadingSpacesChecks) {
    ConsoleMenu menu;
    ostringstream output;
    ostringstream oss;
    for(int i = 0; i < 100; i++) {
        for(int j=0; j < i; j++) {
            oss << " ";
        }
        oss << "POST " << "https://www.example" << i << ".com\n";
    }
    istringstream input(oss.str());

    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin
    IIO * input_io = new StreamIO(""); // Object stands for cin IO

    for(int i = 0; i < 100; i++) {
        output.str(""); // Clear the output stream
        output.clear(); // Clear the output stream state
        EXPECT_EQ(menu.GetCommand(input_io, output), 1);
        EXPECT_EQ(output.str(), "https://www.example" + to_string(i) + ".com\n");
    }

    delete(input_io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
}

TEST(MenuCommand, CorrectCommandWithMenuString) {
    BloomFilter BloomFilter({1, 2, 4}, 16);
    AddCommand* addCommand = new AddCommand();
    CheckCommand* checkCommand = new CheckCommand();
    vector<ICommand*> cmd = {addCommand, checkCommand};
    istringstream input("POST https://www.example.com\n   GET   https://www.example.com\n");
    ostringstream output("");
    
    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output.rdbuf()); // Faking cout
    IIO * io = new StreamIO(""); // Object stands for cin IO

    ostringstream url_buffer_out;
    remove("bloom.txt"); // Safety clean database
    IIO* outFile = new StreamIO("bloom.txt");

    ConsoleMenu menu;
    int cmd1, cmd2;
    cmd1 = menu.GetCommand(io, url_buffer_out);
    EXPECT_EQ(cmd1, 1);
    //cout << "cmd1: " << cmd1 << endl; // debugging
    istringstream url_buffer_in(url_buffer_out.str());
    int addval = cmd[cmd1-1]->execute(BloomFilter, outFile, url_buffer_in);
    EXPECT_EQ(addval, 1);
    url_buffer_out.str("");
    cmd2 = menu.GetCommand(io, url_buffer_out);
    EXPECT_EQ(cmd2, 2);
    url_buffer_in.str(url_buffer_out.str());
    int checkval = cmd[cmd2-1]->execute(BloomFilter, outFile, url_buffer_in);
    EXPECT_EQ(checkval, 4);

    delete(io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    remove("bloom.txt"); // Clean URL file
    delete(outFile); // Clean up the output stream
}

TEST(MenuCommand, IncorrectCommandWithMenuString) {
    BloomFilter BloomFilter({1, 2, 4}, 16);
    AddCommand* addCommand = new AddCommand();
    CheckCommand* checkCommand = new CheckCommand();
    vector<ICommand*> cmd = {addCommand, checkCommand};
    // Using check commands only to succeed to see if some output is printed.
    istringstream input("3 https://www.example.com\n  GEThttps://www.example.com\nGET  \nGET \n\n");
    ostringstream output("");

    // Fake cin from input
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(input.rdbuf()); // Faking cin
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(output.rdbuf()); // Faking cout

    IIO * io = new StreamIO(""); // Object stands for cin IO

    ostringstream url_buffer_out;
    IIO* outFile = new StreamIO("bloom.txt");
    ConsoleMenu menu;
    int cmd1, cmd2;
    // First two inputs are illegal commands.
    cmd1 = menu.GetCommand(io, url_buffer_out);
    EXPECT_EQ(cmd1, 0);
    cmd1 = menu.GetCommand(io, url_buffer_out);
    EXPECT_EQ(cmd1, 0);
    // Next two inputs are illgeal urls.
    cmd1 = menu.GetCommand(io, url_buffer_out);
    EXPECT_EQ(cmd1, 2);
    istringstream url_buffer_in(url_buffer_out.str());
    int val1 = cmd[cmd1-1]->execute(BloomFilter, outFile, url_buffer_in);
    EXPECT_EQ(val1, 0); // Illegal URL so return 0.
    url_buffer_out.str(""); // Reset for next command.

    cmd2 = menu.GetCommand(io, url_buffer_out);
    EXPECT_EQ(cmd2, 2); // Legal command GET.
    url_buffer_in.str(url_buffer_out.str());
    int val2 = cmd[cmd2-1]->execute(BloomFilter, outFile, url_buffer_in);
    EXPECT_EQ(val2, 0); // Illegal URL so return 0.

    
    delete(io); // Clean up the input stream
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete(outFile); // Clean up the output stream
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    // Delete .txt files after running all tests - Created inside test only.
    remove("bloom.txt");
    return result;
    // Splitted the return and test to delete files after tests.
}