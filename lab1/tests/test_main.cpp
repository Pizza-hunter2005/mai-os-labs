#include <gtest/gtest.h>
#include "../include/parent.h"
#include "../include/child.h"

TEST(ParentChildTest, CorrectSumTest1) {
    std::stringstream output;
    std::string filename = "/root/mai-os-labs/lab1/tests/test1.txt";
    std::string expected_output = "16.1\n";
    Parent(filename.c_str(), output);
    ASSERT_EQ(output.str(), expected_output);
}

TEST(ParentChildTest, CorrectSumTest2) {
    std::stringstream output;
    std::string filename = "/root/mai-os-labs/lab1/tests/test2.txt";
    std::string expected_output = "27.1\n";
    Parent(filename.c_str(), output);
    ASSERT_EQ(output.str(), expected_output);
}

TEST(ParentChildTest, EmptyFileTest) {
    std::stringstream output;
    std::string filename = "/root/mai-os-labs/lab1/tests/empty.txt";
    std::string expected_output = "0\n";
    Parent(filename.c_str(), output);
    ASSERT_EQ(output.str(), expected_output);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}