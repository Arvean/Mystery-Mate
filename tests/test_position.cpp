#include <gtest/gtest.h>
#include "position.h"

TEST(PositionTest, CreatePosition) {
    Position d4('d', 4);

    EXPECT_EQ(d4.getFile(), 'd');
    EXPECT_EQ(d4.getRank(), 4);
};