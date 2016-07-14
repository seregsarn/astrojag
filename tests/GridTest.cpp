#include "gtest/gtest.h"
#include "astrojag.h"

using namespace astrojag;

TEST(GridTest, BoundsChecking) {
   Grid<int> test(10,10);
   EXPECT_TRUE(test.inBounds(5,5));
   EXPECT_FALSE(test.inBounds(-1, 5));
   EXPECT_FALSE(test.inBounds( 10, 5));
   EXPECT_FALSE(test.inBounds( 5,-1));
   EXPECT_FALSE(test.inBounds( 5, 10));
}

TEST(GridTest, Clearing) {
    Grid<int> test(10,10);
    test.clearTo(42);
    EXPECT_EQ(test.at(5,5), 42);
}

