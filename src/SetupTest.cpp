#include <gtest/gtest.h>
#include "LineSetup.h"
#include "IDataStructure.h"
#include "BloomFilter.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace Setup;

class LineSetupTest : public ::testing::Test {
protected:
    void SetUp() override {
        ofstream setupOut("test_setup.txt", ios::trunc);
        ofstream urlOut("test_urls.txt", ios::trunc);
        setupOut.close();
        urlOut.close();
    }

    void writeSetupFile(const string& content) {
        ofstream out("test_setup.txt");
        out << content;
        out.close();
    }

    void writeUrlFile(const vector<string>& urls) {
        ofstream out("test_urls.txt");
        for (const auto& url : urls) {
            out << url << "\n";
        }
        out.close();
    }
};

// Valid setup + valid URL file
TEST_F(LineSetupTest, ValidSetupAndUrlFile) {
    writeSetupFile("1000 1 2");
    writeUrlFile({ "http://site1.com", "https://secure.com" });

    LineSetup setup;
    istringstream input_stream("8 2 1\n");
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", input_stream);

    ASSERT_NE(ids, nullptr);
    EXPECT_EQ(ids->CheckUrl("http://site1.com"), 1);
    EXPECT_EQ(ids->CheckUrl("https://secure.com"), 1);
    EXPECT_EQ(ids->CheckUrl("https://notadded.com"), 0);

    delete ids;
}

// Invalid setup file -> prompt input fallback (invalid + valid input)
TEST_F(LineSetupTest, InvalidSetupTriggersInput) {
    writeSetupFile("this is not valid setup");

    std::istringstream fakeInput("not valid\n1000 1 2");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", fakeInput);

    EXPECT_EQ(ids, nullptr);
    delete ids;
}

// Missing setup file -> prompt input fallback
TEST_F(LineSetupTest, MissingSetupTriggersInput) {
    std::istringstream fakeInput("800 2 3");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", fakeInput);
    ASSERT_NE(ids, nullptr);

    delete ids;
}

// Valid setup file, empty URL file
TEST_F(LineSetupTest, ValidSetupEmptyUrlFile) {
    writeSetupFile("512 1 2");

    istringstream input_stream("");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", input_stream);

    ASSERT_NE(ids, nullptr);
    EXPECT_EQ(ids->CheckUrl("http://example.com"), 0);

    delete ids;
}

// Duplicate URLs in file
TEST_F(LineSetupTest, DuplicateUrlsInFile) {
    writeSetupFile("512 3");
    writeUrlFile({ "http://dup.com", "http://dup.com", "http://dup.com" });

    istringstream input_stream("");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", input_stream);

    ASSERT_NE(ids, nullptr);
    EXPECT_EQ(ids->CheckUrl("http://dup.com"), 1);
    EXPECT_EQ(ids->CheckUrl("http://other.com"), 0);

    delete ids;
}

// Incomplete setup file (only array size)
TEST_F(LineSetupTest, IncompleteSetupFile) {
    writeSetupFile("2048");  // no hash functions

    std::istringstream fakeInput("512 1 2");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", fakeInput);
    ASSERT_NE(ids, nullptr);

    delete ids;
}

// Non-integer values in setup file
TEST_F(LineSetupTest, NonIntegerValuesInSetupFile) {
    writeSetupFile("abc def ghi");

    std::istringstream fakeInput("256 2 4");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", fakeInput);
    ASSERT_NE(ids, nullptr);

    delete ids;
}

// Malformed lines in URL file
TEST_F(LineSetupTest, ValidSetupMalformedUrlFile) {
    writeSetupFile("1000 1 2");

    ofstream urlOut("test_urls.txt");
    urlOut << "http://valid.com\n\n\n###\n\nhttps://ok.com\n";
    urlOut.close();

    istringstream input_stream("");

    LineSetup setup;
    IDataStructure* ids = setup.InitializeIDS("test_setup.txt", "test_urls.txt", input_stream);

    ASSERT_NE(ids, nullptr);
    EXPECT_EQ(ids->CheckUrl("http://valid.com"), 1);
    EXPECT_EQ(ids->CheckUrl("https://ok.com"), 1);
    EXPECT_EQ(ids->CheckUrl("https://notadded.com"), 0);

    delete ids;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    // Delete .txt files after running all tests - Created inside test only.
    remove("test_setup.txt");
    remove("test_urls.txt");
    return result;
}