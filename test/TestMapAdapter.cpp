#include "gtest/gtest.h"
#include <map>
#include <aux/MapAdapter.h>
#include <string>

TEST(MapAdapter, Operations)
{
    using map_t = std::map<std::string, std::string>;
    auto map = std::make_shared<map_t>();

    auto vif = std::function<std::string(int)>(
        [](int input) { return std::string("A") + std::to_string(input); });
    auto vof =
        std::function<int(const std::string&)>([](const std::string& input) {
            return std::stoi(std::string(++input.begin(), input.end()));
        });
    auto nop = std::function<std::string(const std::string&)>(
        [](const std::string& input) { return input; });
    auto key_function = std::make_pair(nop, nop);
    auto value_function = std::make_pair(vif, vof);

    auto fmap =
        aux::MapAdapter<map_t, decltype(key_function),
                               decltype(value_function)>(map, key_function,
                                                         value_function);
    fmap.insert(std::make_pair("ONE", 1));
    {
        auto it = map->find("ONE");
        EXPECT_EQ("A1", it->second);
    }
    {
        auto it = fmap.find("ONE");
        EXPECT_EQ(1, it->second);
    }
    {
        auto it = fmap.find("TWO");
        EXPECT_EQ(fmap.end(), it);
    }
    {
        auto it = fmap.begin();
        EXPECT_EQ(fmap.find("ONE"), it);
    }
    fmap.insert(std::make_pair("TWO", 2));
    {
        auto it = map->find("TWO");
        EXPECT_EQ("A2", it->second);
    }
    {
        auto it = fmap.begin();
        it++;
        EXPECT_EQ(2, it->second);
        EXPECT_EQ("TWO", it->first);
        ++it;
        EXPECT_EQ(fmap.end(), it);
        EXPECT_NE(fmap.begin(), it);
    }
    {
        auto it = std::find(fmap.begin(), fmap.end(),
                            std::make_pair(std::string("TWO"), 2));
        EXPECT_EQ(++fmap.begin(), it);
    }
    {
        EXPECT_EQ(std::size_t(1), fmap.erase("ONE"));
        EXPECT_EQ(fmap.end(), fmap.find("ONE"));
        EXPECT_EQ(std::size_t(1), fmap.size());
    }
}
