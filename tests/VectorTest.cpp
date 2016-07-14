#include "gtest/gtest.h"
#include "astrojag.h"

using namespace astrojag;

TEST(VectorTest, Equals) {
   Vector a(1, 2);
   Vector b(1, 2);
   Vector c(1, 3);

   EXPECT_TRUE(a == a);
   EXPECT_TRUE(a == b);
   EXPECT_TRUE(b == a);
   EXPECT_FALSE(a == c);
   EXPECT_FALSE(c == a);
   EXPECT_FALSE(b == c);
   EXPECT_FALSE(c == b);
}

TEST(VectorTest, NotEquals) {
   Vector a(1, 2);
   Vector b(1, 2);
   Vector c(1, 3);

   EXPECT_FALSE(a != b);
   EXPECT_FALSE(b != a);
   EXPECT_TRUE(a != c);
   EXPECT_TRUE(c != a);
   EXPECT_TRUE(b != c);
   EXPECT_TRUE(c != b);
}

TEST(VectorTest, DefaultCtor) {
   Vector a;

   EXPECT_EQ(Vector(0, 0), a);
}

TEST(VectorTest, CopyCtor) {
   Vector a(13, 37);
   Vector b(a);

   EXPECT_EQ(Vector(13, 37), b);
}

TEST(VectorTest, Assignment) {
   Vector a(13, 37);
   Vector b(99, 99);

   EXPECT_EQ(Vector(13, 37), b = a);
   EXPECT_EQ(Vector(13, 37), b);
}

TEST(VectorTest, SelfAssignment) {
   Vector a(13, 37);
   
   a = a;

   EXPECT_EQ(Vector(13, 37), a);
}

TEST(VectorTest, Components) {
   Vector a(13, 37);

   EXPECT_EQ(13, a.x);
   EXPECT_EQ(37, a.y);
}

TEST(VectorTest, Addition) {
   Vector a(7, 11);
   Vector b(13, 19);

   EXPECT_EQ(Vector(7+13, 11+19), a + b);
}

TEST(VectorTest, Subtraction) {
   Vector a(7, 11);
   Vector b(13, 19);

   EXPECT_EQ(Vector(7-13, 11-19), a - b);
}
/*
TEST(VectorTest, Multiplication) {
   Vector a(7, 11);
   Vector b(13, 19);

   EXPECT_EQ(Vector(7*13, 11*19), a * b);
}

TEST(VectorTest, Division) {
   Vector a(100, 15);
   Vector b(20, 3);

   EXPECT_EQ(Vector(100/20, 15/3), a / b);
}
*/
TEST(VectorTest, ScalarMultiplication) {
   Vector a(7, 11);

   EXPECT_EQ(Vector(7*5, 11*5), a * 5.0f);
}

TEST(VectorTest, ScalarDivision) {
   Vector a(100, 25);

   EXPECT_EQ(Vector(100/5, 25/5), a / 5.0f);
}
