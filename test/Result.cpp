#include "gtest/gtest.h"
#include <string>
#include <toolbox/Result.h>

struct CharHash
{
    using result_type = unsigned char;
    using argument_type = std::string;

    unsigned char operator()(const std::string& input) const
    {
        return static_cast<char>(input.at(0));
    }
};

TEST(toolbox, Result)
{
    using Result = toolbox::Result<std::string, CharHash>;
    using Pair = Result::pair_type;
    auto result = Result("elephant");
    result.dirty(false);
    EXPECT_EQ(0, result.get());
    EXPECT_EQ("elephant", result.argument());
    EXPECT_EQ(Pair(0, "elephant"), Pair(result));
    result.argument("panda");
    EXPECT_EQ("panda", result.argument());
    EXPECT_EQ(char('p'), result.get());
    auto result2 = result;
    EXPECT_EQ(result, result2);
    result2.result(42);
    EXPECT_EQ(42u, result2.get());
    EXPECT_EQ("panda", result2.argument());
    EXPECT_NE(result, result2);
    auto result3 = Result(42u, "panda");
    EXPECT_EQ(result2, result3);
    EXPECT_EQ(result2.argument(), result3.argument());
    EXPECT_FALSE(result3.validate());
    EXPECT_EQ(42u, result3.result());
    EXPECT_FALSE(result3.validate());
    auto value = std::string(result3);
    EXPECT_EQ("panda", value);
}
