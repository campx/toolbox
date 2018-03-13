#pragma once
#include <map>
#include <toolbox/HashMap.h>
#include <string>

struct CharHash
{
    using result_type = unsigned char;
    using argument_type = std::string;

    unsigned char operator()(const std::string& input) const;
};

using hash_map_t = toolbox::HashMap<std::string, CharHash>;
