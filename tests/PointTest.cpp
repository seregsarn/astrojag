#include "gtest/gtest.h"
#include "astrojag.h"

using namespace astrojag;

TEST(PointTest, Equals) {
   Point a(1, 2);
   Point b(1, 2);
   Point c(1, 3);

   EXPECT_TRUE(a == a);
   EXPECT_TRUE(a == b);
   EXPECT_TRUE(b == a);
   EXPECT_FALSE(a == c);
   EXPECT_FALSE(c == a);
   EXPECT_FALSE(b == c);
   EXPECT_FALSE(c == b);
}

TEST(PointTest, NotEquals) {
   Point a(1, 2);
   Point b(1, 2);
   Point c(1, 3);

   EXPECT_FALSE(a != b);
   EXPECT_FALSE(b != a);
   EXPECT_TRUE(a != c);
   EXPECT_TRUE(c != a);
   EXPECT_TRUE(b != c);
   EXPECT_TRUE(c != b);
}

TEST(PointTest, LessThan) {
   Point a(1, 1);
   Point b(1, 2);
   Point c(2, 1);
   Point d(2, 2);

   EXPECT_TRUE(a < b);
   EXPECT_TRUE(a < c);
   EXPECT_TRUE(a < d);
   EXPECT_TRUE(b < d);
   EXPECT_TRUE(c < d);
   EXPECT_FALSE(b < a);
   EXPECT_FALSE(c < a);
   EXPECT_FALSE(d < a);
}

TEST(PointTest, DefaultCtor) {
   Point a;

   EXPECT_EQ(Point(0, 0), a);
}

TEST(PointTest, CopyCtor) {
   Point a(13, 37);
   Point b(a);

   EXPECT_EQ(Point(13, 37), b);
}

TEST(PointTest, Assignment) {
   Point a(13, 37);
   Point b(99, 99);

   EXPECT_EQ(Point(13, 37), b = a);
   EXPECT_EQ(Point(13, 37), b);
}

TEST(PointTest, SelfAssignment) {
   Point a(13, 37);
   
   a = a;

   EXPECT_EQ(Point(13, 37), a);
}

TEST(PointTest, Components) {
   Point a(13, 37);

   EXPECT_EQ(13, a.x);
   EXPECT_EQ(37, a.y);
}

TEST(PointTest, Addition) {
   Point a(7, 11);
   Point b(13, 19);

   EXPECT_EQ(Point(7+13, 11+19), a + b);
}

TEST(PointTest, Subtraction) {
   Point a(7, 11);
   Point b(13, 19);

   EXPECT_EQ(Point(7-13, 11-19), a - b);
}

/*
TEST(PointTest, Multiplication) {
   Point a(7, 11);
   Point b(13, 19);

   EXPECT_EQ(Point(7*13, 11*19), a * b);
}

TEST(PointTest, Division) {
   Point a(100, 15);
   Point b(20, 3);

   EXPECT_EQ(Point(100/20, 15/3), a / b);
}
*/

TEST(PointTest, ScalarMultiplication) {
   Point a(7, 11);

   EXPECT_EQ(Point(7*5, 11*5), a * 5);
}

TEST(PointTest, ScalarDivision) {
   Point a(100, 25);

   EXPECT_EQ(Point(100/5, 25/5), a / 5);
}

TEST(PointTest, Distance) {
    Point a(0,0);
    Point b(5,0);
    Point c(3,4);
    EXPECT_EQ(a.distance(b), 5);
    EXPECT_EQ(a.distance(c), 5);
    EXPECT_EQ(a.distance(c), c.distance(a));
    EXPECT_EQ(a.distanceSquared(c), 25);
    EXPECT_EQ(a.manhattan(c), 7);
}
