#include <memory>
#include <stdio.h>
#include "gtest/gtest.h"
#include "astrojag.h"

using namespace astrojag;
using std::shared_ptr;
using std::make_shared;
#include "testMap.h"

TEST(MapTest, creation) {
    auto map = TestingMap::makeTestMap();
    //map->print();
    EXPECT_EQ(map->tiles.at(3,3), 0);
    EXPECT_EQ(map->tiles.at(7,7), 0);
    EXPECT_EQ(map->tiles.at(5,5), 1);
    EXPECT_EQ(map->tiles.at(3,5), 2);
    EXPECT_EQ(map->tiles.at(7,5), 3);
    EXPECT_EQ(map->tiles.at(5,3), 1);
    EXPECT_EQ(map->tiles.at(5,7), 0);
}

TEST(MapTest, FieldOfViewBasic) {
    auto map = TestingMap::makeTestMap();
    FOVMap fov1(map.get(), Point(3,3));
    FOVMap fov2(map.get(), Point(7,7));

    EXPECT_FALSE(fov1.inFOV(7,3));
    EXPECT_TRUE(fov1.inFOV(3,7));
    EXPECT_FALSE(fov1.inFOV(7,7));
    EXPECT_TRUE(fov2.inFOV(3,7));
    EXPECT_FALSE(fov2.inFOV(7,3));
    EXPECT_FALSE(fov2.inFOV(3,3));
}

TEST(MapTest, FieldOfViewCorners) {
    auto map = TestingMap::makeTestMap();
    FOVMap fov1(map.get(), Point(3,3));
    EXPECT_FALSE(fov1.inFOV(6,7));
    EXPECT_TRUE(fov1.inFOV(5,7));

    FOVMap fov2(map.get(), Point(5,1));
    EXPECT_FALSE(fov2.inFOV(4,5));
    EXPECT_FALSE(fov2.inFOV(4,4));
    EXPECT_TRUE(fov2.inFOV(4,3));
    EXPECT_TRUE(fov2.inFOV(4,2));
}

TEST(MapTest, FieldOfViewWalls) {
    auto map = TestingMap::makeTestMap();
    FOVMap fov1(map.get(), Point(4,5));
    EXPECT_FALSE(fov1.inFOV(6,1));
    EXPECT_TRUE(fov1.inFOV(5,1));
}
