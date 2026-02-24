#include <cstdio> 
#include <fstream>
#include <gtest/gtest.h>

#include <sstream>
#include "parent.h"
#include "child.h"


TEST(ParentChildTest, CorrectSumTest1) {
    char temp_input_filename[] = "/root/mai-os-labs/lab1/tests/test1.txt";
    mkstemp(temp_input_filename);
    std::ofstream tmp_file(temp_input_filename);
    tmp_file << "1.1 2.0 3.0\n4.5 5.5\n";
    tmp_file.close();
    std::string expected_output = "16.1\n";
    std::stringstream output;
    Parent(temp_input_filename, output);
    std::string actual_output = output.str();
    EXPECT_EQ(expected_output, actual_output);
    std::remove(temp_input_filename);;
}

TEST(ParentChildTest, CorrectSumTest2) {
    char temp_input_filename[] = "/root/mai-os-labs/lab1/tests/test2.txt";
    mkstemp(temp_input_filename);
    std::ofstream tmp_file(temp_input_filename);
    tmp_file << "1.2 3.4 5.6\n7.8 9.1\n1.0\n\n-1.0";
    tmp_file.close();
    std::string expected_output = "27.1\n";
    std::stringstream output;
    Parent(temp_input_filename, output);
    std::string actual_output = output.str();
    EXPECT_EQ(expected_output, actual_output);
    std::remove(temp_input_filename);
}

TEST(ParentChildTest, EmptyFileTest) {
    char temp_input_filename[] = "/root/mai-os-labs/lab1/tests/empty.txt";
    mkstemp(temp_input_filename);
    std::ofstream tmp_file(temp_input_filename);
    tmp_file << "\n";
    tmp_file.close();
    std::string expected_output = "0\n";
    std::stringstream output;
    Parent(temp_input_filename, output);
    std::string actual_output = output.str();
    EXPECT_EQ(expected_output, actual_output);
    std::remove(temp_input_filename);
}


int main(int argc, char **argv) {
    setenv("CHILD_BINARY_PATH", "/root/mai-os-labs/lab1/build/child", 1);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}