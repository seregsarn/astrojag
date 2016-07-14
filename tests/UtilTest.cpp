#include "gtest/gtest.h"
#include "astrojag.h"

using namespace astrojag;

TEST(UtilTest, Bresenham) {
    Grid<int> test(10,10);
    test.clearTo(0);
    Line(0,0, 9,9, [&](Point& p) {
        test.at(p.x, p.y) += 1;
    });
    EXPECT_EQ(test.at(0,0), 1);
    EXPECT_EQ(test.at(1,1), 1);
    EXPECT_EQ(test.at(2,2), 1);
    EXPECT_EQ(test.at(3,3), 1);
    EXPECT_EQ(test.at(4,4), 1);
    EXPECT_EQ(test.at(5,5), 1);
    EXPECT_EQ(test.at(6,6), 1);
    EXPECT_EQ(test.at(7,7), 1);
    EXPECT_EQ(test.at(8,8), 1);
    EXPECT_EQ(test.at(9,9), 1);

    EXPECT_EQ(test.at(0,1), 0);
    EXPECT_EQ(test.at(1,2), 0);
    EXPECT_EQ(test.at(2,3), 0);
    EXPECT_EQ(test.at(3,4), 0);
    EXPECT_EQ(test.at(4,5), 0);
    EXPECT_EQ(test.at(5,6), 0);
    EXPECT_EQ(test.at(6,7), 0);
    EXPECT_EQ(test.at(7,8), 0);
    EXPECT_EQ(test.at(8,9), 0);
    EXPECT_EQ(test.at(1,0), 0);
    EXPECT_EQ(test.at(2,1), 0);
    EXPECT_EQ(test.at(3,2), 0);
    EXPECT_EQ(test.at(4,3), 0);
    EXPECT_EQ(test.at(5,4), 0);
    EXPECT_EQ(test.at(6,5), 0);
    EXPECT_EQ(test.at(7,6), 0);
    EXPECT_EQ(test.at(8,7), 0);
    EXPECT_EQ(test.at(9,8), 0);
}

/*
    PriorityQueue();
    ~PriorityQueue();
    void clear();
    int size() { return backingStore.size(); }
    void insert(const int priority, const T value);
    void push(const int priority, const T value) { insert(priority, value); }
    typename PriorityQueue<T>::Node popNode();
    T pop();
*/
TEST(UtilTest, PriorityQueueTests) {
    PriorityQueue<std::string> pq;
    std::string val;
    
    pq.insert(2, "two");
    pq.insert(1, "one");
    pq.insert(5, "five");
    pq.push(4, "four");
    pq.insert(3, "three");
    EXPECT_EQ(pq.size(), 5);

    val = pq.pop();
    EXPECT_EQ(val, "one");
    val = pq.pop();
    EXPECT_EQ(val, "two");
    val = pq.pop();
    EXPECT_EQ(val, "three");
    pq.insert(1, "one-point-five");
    val = pq.pop();
    EXPECT_EQ(val, "one-point-five");
    val = pq.pop();
    EXPECT_EQ(val, "four");
    val = pq.pop();
    EXPECT_EQ(val, "five");
    
    EXPECT_EQ(pq.size(), 0);
}
