#include "gtest/gtest.h"
#include <toolbox/HashMap.h>
#include <map>
#include <string>

TEST(HashMap, Operations)
{
    using hash_t = std::hash<std::string>;
    using map_t = std::map<hash_t::result_type, std::string>;
    using hash_map_t = toolbox::HashMap<hash_t, map_t>;

    auto hash_map = hash_map_t{};
    auto inserted = hash_map.insert("elephant");
    auto key = inserted.first->first;
    EXPECT_EQ(true, inserted.second);
    EXPECT_EQ(10238581130556107085u, inserted.first->first);
    EXPECT_EQ(inserted.first, hash_map.find(key));

    using pair_vector_t = std::vector<std::pair<int, std::string>>;
    pair_vector_t results;
    std::copy(hash_map.cbegin(), hash_map.cend(), std::back_inserter(results));
    EXPECT_EQ(pair_vector_t::size_type(1), results.size());
    EXPECT_EQ(hash_map_t::size_type(1), hash_map.erase(key));
}
