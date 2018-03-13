#include "HashMap.h"
#include "gtest/gtest.h"
#include <algorithm>

unsigned char CharHash::operator()(const std::string& input) const
{
    return std::accumulate(input.begin(), input.end(), 0, std::plus<char>());
}

TEST(HashMap, Operations)
{

    auto hash_map = hash_map_t{};
    auto inserted = hash_map.insert("elephant");
    auto key = inserted.first->first;
    EXPECT_EQ(true, inserted.second);
    EXPECT_EQ('Q', inserted.first->first);
    EXPECT_EQ(inserted.first, hash_map.find(key));

    using pair_vector_t = std::vector<std::pair<int, std::string>>;
    pair_vector_t results;
    std::copy(hash_map.cbegin(), hash_map.cend(), std::back_inserter(results));
    EXPECT_EQ(pair_vector_t::size_type(1), results.size());
    EXPECT_EQ(hash_map_t::size_type(1), hash_map.erase(key));
}
