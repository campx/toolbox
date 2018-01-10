#include "gtest/gtest.h"
#include <memory>
#include <set>
#include <toolbox/Value.h>

TEST(Toolbox, Value)
{
    const auto v1 = toolbox::Value<int>(1);
    auto v2 = v1;
    EXPECT_EQ(v1, v2);
    v2 = 4;
    EXPECT_NE(v1, v2);
    EXPECT_GT(v2, v1);
    EXPECT_LT(v1, v2);
    EXPECT_EQ(1, *(v1.get()));
    EXPECT_EQ(1, *v1);

    auto set = toolbox::Value<std::shared_ptr<std::set<int>>>();
    EXPECT_EQ(nullptr, set.get());
    set = std::make_shared<std::set<int>>();
    EXPECT_EQ(std::size_t(0), set->size());
    set->insert(1);
    EXPECT_EQ(std::size_t(1), set->size());
}
