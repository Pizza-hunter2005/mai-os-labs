#include <gtest/gtest.h>
#include "PrimeCount.h"
#include "Square.h"


TEST(PrimeCountTests, NaiveMethod) {
    EXPECT_EQ(PrimeCountNaive(1, 10), 4);
    EXPECT_EQ(PrimeCountNaive(11, 20), 4);
    EXPECT_EQ(PrimeCountNaive(0, 1), 0); 
}

TEST(PrimeCountTests, SieveMethod) {
    EXPECT_EQ(PrimeCountSieve(1, 10), 4);
    EXPECT_EQ(PrimeCountSieve(11, 20), 4);
    EXPECT_EQ(PrimeCountSieve(0, 1), 0);
}


TEST(SquareTests, RectangleArea) {
    EXPECT_FLOAT_EQ(SquareRectangle(3.0f, 4.0f), 12.0f); 
    EXPECT_FLOAT_EQ(SquareRectangle(5.5f, 2.0f), 11.0f); 
    EXPECT_FLOAT_EQ(SquareRectangle(0.0f, 4.0f), 0.0f); 
}

TEST(SquareTests, TriangleArea) {
    EXPECT_FLOAT_EQ(SquareTriangle(3.0f, 4.0f), 6.0f);
    EXPECT_FLOAT_EQ(SquareTriangle(5.5f, 2.0f), 5.5f);
    EXPECT_FLOAT_EQ(SquareTriangle(0.0f, 4.0f), 0.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}