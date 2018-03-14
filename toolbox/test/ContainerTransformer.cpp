#include "gtest/gtest.h"
#include <string>
#include <toolbox/ContainerTransformer.h>
#include <functional>

TEST(Toolbox, ContainerTransformer)
{
    /** Convert string to int */
    auto input_function = std::function<int(const std::string&)>(
        [](const std::string& input) { return std::atoi(input.c_str()); });
    /** Convert int to string */
    auto output_function = std::function<std::string(int)>(
        [](int input) { return std::to_string(input); });
    auto io = std::make_pair(input_function, output_function);
    {
        EXPECT_EQ(1, input_function("1"));
        EXPECT_EQ("1", output_function(1));

        /** Adapt a set of ints into a set of strings */
        auto set = toolbox::ContainerTransformer<std::set<int>, decltype(io)>(
            std::set<int>{}, io);
        set.insert("1");
        EXPECT_NE(set.end(), set.find("1"));
    }
    {
        /** Check that ContainerTransformer also works when the container is a
         * smart pointer type */
        using Set = std::shared_ptr<std::set<int>>;
        auto pset = std::make_shared<std::set<int>>();
        auto set = toolbox::ContainerTransformer<Set, decltype(io)>(pset, io);
        set.insert("1");
        EXPECT_NE(set.end(), set.find("1"));
        EXPECT_NE(pset->cend(), pset->find(1));
    }
    {
        struct InputFunction
        {
            int operator()(const int& input) const
            {
                return input;
            }
            std::pair<int, std::string>
            operator()(const std::pair<int, std::string>& input) const
            {
                return std::make_pair(std::atoi(input.second.c_str()),
                                      input.second);
            }
        };
        struct OutputFunction
        {
            int operator()(const int& input) const
            {
                return input;
            }
            std::pair<int, std::string>
            operator()(const std::pair<int, std::string>& input) const
            {
                return input;
            }
        };
        using map_t = std::map<int, std::string>;
        auto map = toolbox::ContainerTransformer<
            map_t, std::pair<InputFunction, OutputFunction>>();
        map.insert(std::make_pair(0, "1234")); // 0 is overwritten with 1234
        auto it = map.find(1234);
        EXPECT_NE(map.cend(), it);
        EXPECT_EQ(it->second, "1234");
    }
}
