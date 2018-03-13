#include "gtest/gtest.h"
#include <toolbox/Iterator.h>
#include <vector>

TEST(Toolbox, Iterator)
{
    auto input = std::vector<std::string>{"a", "b", "c", "d", "e"};
    EXPECT_EQ("e", *toolbox::last(input));
}
