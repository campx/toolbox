#include "gtest/gtest.h"
#include <string>
#include <toolbox/LazyEvaluation.h>

struct CharHash
{
    using result_type = unsigned char;
    using argument_type = std::string;

    unsigned char operator()(const std::string& input) const
    {
        return static_cast<char>(input.at(0));
    }
};

TEST(toolbox, LazyEvaluation)
{
    using LazyEvaluation = toolbox::LazyEvaluation<std::string, CharHash>;
    auto result = LazyEvaluation("elephant");
    result.dirty(false);
    EXPECT_EQ(0, result.get());
    EXPECT_EQ("elephant", result.argument());
    EXPECT_EQ(0, result.result());
    EXPECT_EQ("elephant", result.argument());
    result.argument("panda");
    EXPECT_EQ("panda", result.argument());
    EXPECT_EQ(char('p'), result.get());
    auto result2 = result;
    EXPECT_EQ(result, result2);
    result2.result(42);
    EXPECT_EQ(42u, result2.get());
    EXPECT_EQ("panda", result2.argument());
    EXPECT_NE(result, result2);
    auto result3 = LazyEvaluation("panda");
    result3.result(result2.result());
    EXPECT_EQ(result2, result3);
    EXPECT_EQ(result2.argument(), result3.argument());
    EXPECT_FALSE(result3.validate());
    EXPECT_EQ(42u, result3.result());
    EXPECT_FALSE(result3.validate());
    EXPECT_EQ("panda", result3.argument());
}
