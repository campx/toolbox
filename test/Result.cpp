#include "gtest/gtest.h"
#include <toolbox/HashKeyValue.h>
#include <string>

struct CharHash
{
    using result_type = unsigned char;
    using argument_type = std::string;

    unsigned char operator()(const std::string& input) const
    {
        return static_cast<char>(input.at(0));
    }
};

TEST(toolbox, HashKeyValue)
{
    using KeyValue = toolbox::HashKeyValue<std::string, CharHash>;
    using Pair = KeyValue::pair_type;
    auto key_value = KeyValue("elephant");
    EXPECT_EQ(char('Q'), key_value.key());
    EXPECT_EQ("elephant", key_value.value());
    EXPECT_EQ(Pair(char('Q'), "elephant"), Pair(key_value));
    key_value.value("panda");
    EXPECT_EQ("panda", key_value.value());
    EXPECT_EQ(char(4), key_value.key());
    auto key_value2 = key_value;
    EXPECT_EQ(key_value, key_value2);
    key_value2.key(42);
    EXPECT_EQ(42u, key_value2.key());
    EXPECT_EQ("panda", key_value2.value());
    EXPECT_NE(key_value, key_value2);
    auto key_value3 = KeyValue(42u, "panda");
    EXPECT_EQ(key_value2, key_value3);
    EXPECT_EQ(key_value2.value(), key_value3.value());
    EXPECT_FALSE(key_value3.validate());
    key_value3.rehash();
    EXPECT_TRUE(key_value3.validate());
    auto value = std::string(key_value3);
    EXPECT_EQ("panda", value);
}
