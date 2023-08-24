#include "gtest/gtest.h"
#include "position.h"

TEST(PositionTests, DefaultConstructor) {
    Position position;

    EXPECT_EQ(position.getRank(), 0);
    EXPECT_EQ(position.getFile(), 0);
}

TEST(PositionTests, ParameterizedConstructor) {
    Position position('a', 1);
    EXPECT_EQ(position.getRank(), 1);
    EXPECT_EQ(position.getFile(), 'a');
}

TEST(PositionTests, EqualityOperator) {
    Position position1('b', 2);
    Position position2('b', 2);
    Position position3('c', 3);

    EXPECT_TRUE(position1 == position2);
    EXPECT_FALSE(position1 == position3);
}

TEST(PositionTests, GetRank) {
    Position position('d', 4);
    EXPECT_EQ(position.getRank(), 4);
}

TEST(PositionTests, GetFile) {
    Position position('e', 5);
    EXPECT_EQ(position.getFile(), 'e');
}

TEST(PositionTests, HashFunction) {
    Position position1('f', 6);
    Position position2('f', 6);

    std::hash<Position> hash_fn;
    EXPECT_EQ(hash_fn(position1), hash_fn(position2));
}
