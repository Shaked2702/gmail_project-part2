// Note: First test is truncating the url_test.txt file.
// Note: Output streams still exists in all tests in case of testing console outputs (FakeCout+OriginalCoutBuf)
#include <gtest/gtest.h>
#include "HashFunctions.h"
#include "BloomFilter.h"
#include "IDataStructure.h"
#include "ICommand.h"
#include "AddCommand.h"
#include "CheckCommand.h"
#include "DeleteCommand.h"
#include <iostream>
#include <vector>
#include <iterator>
#include "IIO.h"
#include "StreamIO.h"
using namespace std;
using namespace Command;

/*MODIFICATION TO ALL TEST INTO STREAM FRIENDLY VERSION*/
// TEST ILLEGAL URL RETURN FALSE FOR THE ADD COMMAND
TEST(ICommand, IllegalUrlAdd) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;

    // Faking values inside cin : 5 illegal urls.
    string fakeInput = "https//www.example.com\nhtps://www.example.com\nhttp://w\nhttps://.c\nhttps://www.ex .com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Fake cin cout
    streambuf* orig_buf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* orig_buf_out = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout

    IIO* io = new StreamIO(""); // Object stands for cin cout IO

    IIO* url_file = new StreamIO("url_test.txt");
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 0) << "AddCommand should not be executed";
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 0) << "AddCommand should not be executed";
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 0) << "AddCommand should not be executed";
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 0) << "AddCommand should not be executed";
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 0) << "AddCommand should not be executed";
    
    // Restore original cin and cout
    cin.rdbuf(orig_buf);  // Restore original cin
    cout.rdbuf(orig_buf_out);  // Restore original cout
    delete(io); // Clean up the input stream

    delete(url_file); // Close the file
    remove("url_test.txt"); // Remove the test file
}

// TEST ILLEGAL URL GIVE FALSE FOR THE CHECK COMMAND
TEST(ICommand, IllegalUrlCheck) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin : 5 illegal urls.
    string fakeInput = "https//www.example.com\nhtps://www.example.com\nhttp://w\nhttps://w. com\nhttps://www.ex .com\n";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");
    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");

    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 0) << "CheckCommand should not be executed";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 0) << "CheckCommand should not be executed";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 0) << "CheckCommand should not be executed";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 0) << "CheckCommand should not be executed";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 0) << "CheckCommand should not be executed";
    delete(url_file); // Close the file

    // Restore original cin and cout
    cin.rdbuf(originalCinBuf);
    cout.rdbuf(originalCoutBuf);
    delete io; // Clean up the input stream

    remove("url_test.txt"); // Remove the test file
}

// TEST URL WITH SPACES AT START OR END (IN MIDDLE OF URL IS ILLEGAL)
TEST(ICommand, StartEndSpacedUrl) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin.
    string fakeInput = "  https://www.example.com\n      https://www.example.com\nhttps://www.example.com    ";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");
    
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 1) << "addCommand should be executed";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 4) << "CheckCommand return true, ignore spaces before";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 4) << "CheckCommand return true, ignore spaces after";

    delete(url_file); // Close the file

    // Restore original cin and cout
    cin.rdbuf(originalCinBuf);
    cout.rdbuf(originalCoutBuf);
    delete io; // Clean up the input stream

    remove("url_test.txt"); // Remove the test file
}


// TEST BASIC USAGE OF COMMANDS
TEST(ICommand, CommandUsageBasic) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin
    string fakeInput = "https://www.example.com\nhttps://www.example.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");

    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 1) << "AddCommand should be executed successfully";
    EXPECT_EQ(checkCommand.execute(BloomFilter, url_file, cin), 4) << "CheckCommand should be executed successfully";

    delete(url_file); // Close the file
    
    // Restore original cin and cout
    cin.rdbuf(originalCinBuf);
    cout.rdbuf(originalCoutBuf);
    delete io; // Clean up the input stream

    remove("url_test.txt"); // Remove the test file
}

TEST(ICommand, CommandAddTwice) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin
    string fakeInput = "https://www.example.com\nhttps://www.example.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");

    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 1) << "AddCommand should be executed successfully";
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, cin), 2) << "AddCommand should be executed successfully, URL was already in Bloom";
    delete(url_file); // Close the file
    
    // Restore original cin and cout
    cin.rdbuf(originalCinBuf);
    cout.rdbuf(originalCoutBuf);
    delete io; // Clean up the input stream
    remove("url_test.txt"); // Remove the test file
}

// TEST USAGE OF COMMANDS WITH INHERITENCE TO ICOMMAND
TEST(ICommand, InheritenceCheck) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    ICommand* cmd1 = &addCommand;
    ICommand* cmd2 = &checkCommand;
    
    // Faking values inside cin
    string fakeInput = "https://www.example.com\nhttps://www.example.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");

    EXPECT_EQ(cmd1->execute(BloomFilter, url_file, cin), 1) << "AddCommand should be executed successfully";
    EXPECT_EQ(cmd2->execute(BloomFilter, url_file, cin), 4) << "CheckCommand should be executed successfully";
    delete(url_file); // Close the file

    // Restore original cin and cout
    std::cin.rdbuf(originalCinBuf);
    std::cout.rdbuf(originalCoutBuf);
    delete io; // Clean up the input stream
    remove("url_test.txt"); // Remove the test file
}

// TEST SAVING URL ADDED INTO FILE - NO DUPLICATIONS TEST ALSO
TEST(ICommand, UrlFileSavingCheck) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    
    // Faking values inside cin
    string fakeInput = "https://www.example.com\nhttps://www.example.com\nhttps://www.example2.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, cin);
    addCommand.execute(BloomFilter, url_file, cin);
    addCommand.execute(BloomFilter, url_file, cin);

    delete(url_file); // Close the file

    string expected = "https://www.example.com\nhttps://www.example2.com\n";
    ifstream infile("url_test.txt", ios::in);
    stringstream buffer;
    buffer << infile.rdbuf();
    string content = buffer.str();
    infile.close();
    EXPECT_EQ(content, expected) << "File should contain URLs without duplications";

    // Restore original cin and cout
    cin.rdbuf(originalCinBuf);
    cout.rdbuf(originalCoutBuf);
    delete io; // Clean up the input stream
    remove("url_test.txt"); // Remove the test file
}

// CHECK MULTIPLE USAGE OF COMMANDS
TEST(ICommand, MultipleCommands) {
    BloomFilter BloomFilter({1, 2}, 8);
    ICommand* arr[64];
    for (int i = 0; i < 32; i++) {
        arr[i] = new AddCommand();
    }
    for (int i = 32; i < 64; i++) {
        arr[i] = new CheckCommand();
    }
    // Backup once the cin cout
    streambuf* originalCinBuf = std::cin.rdbuf();
    streambuf* originalCoutBuf = std::cout.rdbuf();

    // Faking values inside cin
    for (int i = 0; i < 64; i++) {
        string fakeInput = "https://www.example" + to_string(i%32) + ".com";
        istringstream fakeCin(fakeInput);
        ostringstream fakeCout("");
        // Fake cin cout
        cin.rdbuf(fakeCin.rdbuf());
        cout.rdbuf(fakeCout.rdbuf());
        IIO* io = new StreamIO(""); // Object stands for cin cout IO
        IIO* url_file = new StreamIO("url_test.txt");

        int output = arr[i]->execute(BloomFilter, url_file, cin);
        if (i < 32) {
            EXPECT_EQ(output, 1) << "AddCommand should be executed";
        } else {
            EXPECT_EQ(output, 4) << "All 32 adds should work, All 32 checks should return true (or true false).";
        }
        delete(url_file); // Close the file
    }
    cin.rdbuf(originalCinBuf); // Restore the original cin buffer
    cout.rdbuf(originalCoutBuf); // Restore the original cout buffer
    remove("url_test.txt"); // Remove the test file
}

//TEST THE OUTPUTS OF CHECK COMMAND - FALSE POSITIVE
TEST(ICommand, OutputStreamCheckFalsePositive) {
    BloomFilter BloomFilter({1, 2}, 1);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin.
    string fakeInput = "https://www.example.com\nhttps://www.example2.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");
    
    // Fake cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf()); // Faking cin
    streambuf* originalCoutBuf = cout.rdbuf();
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, cin);
    int fp = checkCommand.execute(BloomFilter, url_file, cin);

    EXPECT_EQ(fp, 5) << "Output should be 5 = 'true false' for false positive";
    delete(url_file); // Close the file

    // Restore original cin and cout
    cin.rdbuf(originalCinBuf); // Restore the original cin buffer
    cout.rdbuf(originalCoutBuf); // Restore the original cout buffer
    //cout << "Check cout back to normal.\n"; // debbuging
    remove("url_test.txt"); // Remove the test file
}

//TEST THE OUTPUTS OF CHECK COMMAND - TRUE POSITIVE
TEST(ICommand, OutputCheckTruePositive) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin.
    string fakeInput = "https://www.example.com\nhttps://www.example.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Reddirecting cin cout to the buffer.
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf());
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, cin);
    int tp = checkCommand.execute(BloomFilter, url_file, cin);
    EXPECT_EQ(tp, 4) << "Output should be 4 = 'true true' for true positive";

    delete(url_file); // Close the file

    // Return cin cout to original.
    cin.rdbuf(originalCinBuf); // Restore the original cin buffer
    cout.rdbuf(originalCoutBuf); // Restore the original cout buffer
    delete(io);
    remove("url_test.txt"); // Remove the test file
}

//TEST THE OUTPUTS OF CHECK COMMAND - NEGATIVE
//Note: 1/(16^4) probability of test fail (for false positive).
TEST(ICommand, OutputCheckNegative) {
    BloomFilter BloomFilter({1, 2, 4, 4}, 16);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin.
    string fakeInput = "https://www.example.com\nhttps://www.example2.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Change cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf()); // Faking cin
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, cin);
    int fal = checkCommand.execute(BloomFilter, url_file, cin);
    EXPECT_EQ(fal, 3) << "Output should be 3 = 'false' for negative";

    delete(url_file); // Close the file
    
    // Restore original cin and cout
    cin.rdbuf(originalCinBuf); // Restore the original cin buffer
    cout.rdbuf(originalCoutBuf); // Restore the original cout buffer
    delete(io); // Clean up the input stream
    remove("url_test.txt"); // Remove the test file
}

// TEST THE OUTPUTS OF CHECK COMMAND - TO FILE STREAM
TEST(ICommand, OutputURLToFileStream) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    
    // Faking values inside cin.
    string fakeInput = "https://www.example.com\nhttps://www.example.com";
    istringstream fakeCin(fakeInput);
    ostringstream fakeCout("");

    // Change cin cout
    streambuf* originalCinBuf = cin.rdbuf(); // Backup cin
    cin.rdbuf(fakeCin.rdbuf()); // Faking cin
    streambuf* originalCoutBuf = cout.rdbuf(); // Backup cout
    cout.rdbuf(fakeCout.rdbuf()); // Faking cout
    IIO* io = new StreamIO(""); // Object stands for cin cout IO
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, cin);
    checkCommand.execute(BloomFilter, url_file, cin);
    
    delete(url_file); // Close the file
    // Read from url_test.txt into buffer string
    stringstream buffer;
    ifstream infile("url_test.txt");
    buffer << infile.rdbuf();
    string content = buffer.str();
    infile.close();

    EXPECT_EQ(content, "https://www.example.com\n") << "Output should include url added to file.";

    cin.rdbuf(originalCinBuf); // Restore the original cin buffer
    cout.rdbuf(originalCoutBuf); // Restore the original cout buffer
    delete io; // Clean up the input stream
}

/* -----------------------------------Delete Suite-----------------------------------*/
// Test Delete Command Basic Usage
TEST(DeleteCommand, BasicDelete)
{
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    DeleteCommand deleteCommand;
    
    // Faking values inside cin
    string fakeInput = "https://www.example.com\nhttps://www.example.com\nhttps://www.example.com\nhttps://www.example.com";
    istringstream fakeStream(fakeInput);
    
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, fakeStream);
    int che = checkCommand.execute(BloomFilter, url_file, fakeStream);
    EXPECT_EQ(che, 4) << "CheckCommand should be executed successfully";
    int del = deleteCommand.execute(BloomFilter, url_file, fakeStream);
    EXPECT_EQ(del, 7) << "DeleteCommand should be executed successfully";
    int che2 = checkCommand.execute(BloomFilter, url_file, fakeStream);
    EXPECT_EQ(che2 == 3 || che2 == 5, true) << "CheckCommand should be executed successfully - URL deleted";

    delete(url_file); // Close the file
    
    remove("url_test.txt"); // Remove the test file
}

// Test Multiple Url Delete Commands
TEST(DeleteCommand, MultipleDelete)
{
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    DeleteCommand deleteCommand;
    string fakeInput = "";
    // Faking values inside cin
    for (int i = 0; i < 16; i++) {
        fakeInput = fakeInput + "https://www.example.com\n";
    }
    istringstream fakeStream(fakeInput);
    
    IIO* url_file = new StreamIO("url_test.txt");
    
    // Add URL and Delete URL each time check that exist after add and not exist after delete.
    // Each iteration 4 commands so 4 times
    for (int i = 0; i < 4; i++) {
        addCommand.execute(BloomFilter, url_file, fakeStream);
        int che = checkCommand.execute(BloomFilter, url_file, fakeStream);
        EXPECT_EQ(che, 4) << "CheckCommand should retrun URL is inside Bloom";
        int del = deleteCommand.execute(BloomFilter, url_file, fakeStream);
        EXPECT_EQ(del, 7) << "DeleteCommand should be deleting successfully";
        int che2 = checkCommand.execute(BloomFilter, url_file, fakeStream);
        EXPECT_EQ(che2 == 3 || che2 == 5, true) << "CheckCommand should be executed successfully - URL deleted";
    }
    delete(url_file); // Close the file
    remove("url_test.txt"); // Remove the test file
}

// Test Delete Command with Illegal URL (regex) - NO logical meaning
TEST(DeleteCommand, IllegalUrlDelete)
{
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    DeleteCommand deleteCommand;
    
    // Faking values inside cin
    string fakeInput = "https//www.examp le.com\nhtps://www.example.com\nhttp://w\nhttps://.c\nhttps://www.ex .com";
    istringstream fakeStream(fakeInput);
    
    IIO* url_file = new StreamIO("url_test.txt");
    
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 0) << "DeleteCommand should not be executed";
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 0) << "DeleteCommand should not be executed";
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 0) << "DeleteCommand should not be executed";
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 0) << "DeleteCommand should not be executed";
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 0) << "DeleteCommand should not be executed";
    
    delete(url_file); // Close the file
    remove("url_test.txt"); // Remove the test file
}

// Test Delete Command with URL that is not inside the Bloom
TEST(DeleteCommand, UrlNotInBloom)
{
    // 1) Delete non added url. 2) Delete added&deleted url.
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    DeleteCommand deleteCommand;
    
    // Faking values inside cin - 4 vals - 1) Check non added url. 2) Check added&deleted url.
    string fakeInput = "https://www.example.com\nhttps://www.example1.com\nhttps://www.example1.com\nhttps://www.example1.com";
    istringstream fakeStream(fakeInput);
    
    IIO* url_file = new StreamIO("url_test.txt");
    
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 8) << "DeleteCommand should not be executed - URL not in Bloom";
    EXPECT_EQ(addCommand.execute(BloomFilter, url_file, fakeStream), 1) << "AddCommand should be executed successfully";
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 7) << "DeleteCommand should be executed successfully";
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 8) << "DeleteCommand should not be executed - URL not in Bloom";

    delete(url_file); // Close the file
    remove("url_test.txt"); // Remove the test file
}

// Test File Stream Has Removed Url
TEST(DeleteCommand, FileStreamUrlRemoved) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    DeleteCommand deleteCommand;
    
    // Faking values inside cin - add, check url in file, delete url, check url not in file
    string fakeInput = "https://www.example.com\nhttps://www.example.com\nhttps://www.example.com\nhttps://www.example.com";
    istringstream fakeStream(fakeInput);
    
    IIO* url_file = new StreamIO("url_test.txt");
    
    addCommand.execute(BloomFilter, url_file, fakeStream);
    url_file->close(); // Close the file stream
    // Check url added to file
    string expected = "https://www.example.com\n";
    ifstream infile("url_test.txt", ios::in);
    stringstream buffer;
    buffer << infile.rdbuf();
    string content = buffer.str();
    infile.close();
    EXPECT_EQ(content, expected) << "File should contain URL added to file";

    url_file->open(); // Open the file stream back again
    // Delete url with command
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 7) << "DeleteCommand should be executed successfully";
    url_file->close(); // Close the file stream
    // Check url deleted from file
    ifstream infile2("url_test.txt", ios::in);
    stringstream buffer2;
    buffer2 << infile2.rdbuf();
    string content2 = buffer2.str();
    infile2.close();
    EXPECT_EQ(content2, "") << "File should not contain URL deleted from file";
    
    delete(url_file); // Close the file
    remove("url_test.txt"); // Remove the test file
}

// Test File Stream Url Removed Multiple Urls
TEST(DeleteCommand, FileStreamUrlRemovedMultiple) {
    BloomFilter BloomFilter({1, 2}, 8);
    AddCommand addCommand;
    CheckCommand checkCommand;
    DeleteCommand deleteCommand;
    
    string fakeInput = "";
    // Add into a fake stream 10 different urls
    for (int i = 0; i < 10; i++) {
        fakeInput = fakeInput + "https://www.example" + to_string(i) + ".com\n";
    }
    // Add first, middle, last url to the fake stream to check each deletion.
    fakeInput = fakeInput + "https://www.example5.com\nhttps://www.example0.com\nhttps://www.example9.com\n";

    istringstream fakeStream(fakeInput);

    IIO* url_file = new StreamIO("url_test.txt");

    // Add 10 url to the file
    for (int i = 0; i < 10; i++) {
        addCommand.execute(BloomFilter, url_file, fakeStream);
    }
    // Check the content of the file
    url_file->close(); // Close the file stream
    string expected = "https://www.example0.com\nhttps://www.example1.com\nhttps://www.example2.com\nhttps://www.example3.com\nhttps://www.example4.com\nhttps://www.example5.com\nhttps://www.example6.com\nhttps://www.example7.com\nhttps://www.example8.com\nhttps://www.example9.com\n";
    ifstream infile("url_test.txt", ios::in);
    stringstream buffer;
    buffer << infile.rdbuf();
    string content = buffer.str();
    infile.close();
    EXPECT_EQ(content, expected) << "File should contain URL added to file";
    // Open the file stream back again
    url_file->open();

    // Delete mid URL and check URL is not in file
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 7) << "DeleteCommand should be executed successfully";
    url_file->close(); // Close the file stream

    ifstream infile2("url_test.txt", ios::in);
    stringstream buffer2;
    buffer2 << infile2.rdbuf();
    string content2 = buffer2.str();
    infile2.close();
    string expected2 = "https://www.example0.com\nhttps://www.example1.com\nhttps://www.example2.com\nhttps://www.example3.com\nhttps://www.example4.com\nhttps://www.example6.com\nhttps://www.example7.com\nhttps://www.example8.com\nhttps://www.example9.com\n";
    EXPECT_EQ(content2, expected2) << "File should not contain URL deleted from file";

    // Open the file stream back again
    url_file->open();
    // Delete first URL and check URL is not in file
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 7) << "DeleteCommand should be executed successfully";
    url_file->close(); // Close the file stream
    ifstream infile3("url_test.txt", ios::in);
    stringstream buffer3;
    buffer3 << infile3.rdbuf();
    string content3 = buffer3.str();
    infile3.close();
    string expected3 = "https://www.example1.com\nhttps://www.example2.com\nhttps://www.example3.com\nhttps://www.example4.com\nhttps://www.example6.com\nhttps://www.example7.com\nhttps://www.example8.com\nhttps://www.example9.com\n";
    EXPECT_EQ(content3, expected3) << "File should not contain URL deleted from file";

    // Open the file stream back again
    url_file->open();
    // Delete last URL and check URL is not in file
    EXPECT_EQ(deleteCommand.execute(BloomFilter, url_file, fakeStream), 7) << "DeleteCommand should be executed successfully";
    url_file->close(); // Close the file stream
    ifstream infile4("url_test.txt", ios::in);
    stringstream buffer4;
    buffer4 << infile4.rdbuf();
    string content4 = buffer4.str();
    infile4.close();
    string expected4 = "https://www.example1.com\nhttps://www.example2.com\nhttps://www.example3.com\nhttps://www.example4.com\nhttps://www.example6.com\nhttps://www.example7.com\nhttps://www.example8.com\n";
    EXPECT_EQ(content4, expected4) << "File should not contain URL deleted from file";
    
    delete(url_file); // Close the file
    remove("url_test.txt"); // Remove the test file        
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    // Delete .txt files after running all tests - Created inside test only.
    remove("url_test.txt");
    remove("console_test.txt");
    return result;
    // Splitted the return and test to delete files after tests.
}