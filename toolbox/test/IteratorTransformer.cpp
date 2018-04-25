#include "gtest/gtest.h"
#include <toolbox/Iterator.h>
#include <toolbox/IteratorTransformer.h>

struct Transform
{
    std::string operator()(int value)
    {
        return std::to_string(value * 10);
    }

    int operator()(const std::string& value)
    {
        return std::atoi(value.c_str()) / 10;
    }
};

TEST(Toolbox, IteratorTransformer)
{
    {
        using Input = std::vector<int>;
        using Output = std::vector<std::string>;
        using Iterator = Input::const_iterator;
        using Transformer = toolbox::IteratorTransformer<Transform, Iterator>;
        auto input = Input{1, 2, 3, 4, 5, 6, 7, 8};
        auto output = Output{"10", "20", "30", "40", "50", "60", "70", "80"};
        auto transformer = Transformer{input.cbegin()};
        EXPECT_EQ(input.cbegin(), transformer.get());
        EXPECT_EQ("10", *transformer);
        EXPECT_EQ("20", *(++transformer));
        EXPECT_EQ(2u, transformer->size());
        auto result = Output{};
        auto begin = Transformer(input.cbegin());
        auto end = Transformer(input.cend());
        std::copy(begin, end, std::back_inserter(result));
        EXPECT_EQ(output, result);
    }
}
