#include "gtest/gtest.h"
#include <toolbox/Codec.h>

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

TEST(Toolbox, Codec)
{
    {
        auto transform = Transform();
        auto codec = Codec<Transform, Transform>(transform, transform);
        EXPECT_EQ(1, codec.decode("10"));
        EXPECT_EQ("10", codec.encode(1));
    }
    {
        auto codec = makeCodec([](int v) { return std::to_string(v * 10); },
                               [](const std::string& s) {
                                   return std::atoi(s.c_str()) / 10;
                               });
        EXPECT_EQ("10", codec.encode(1));
        EXPECT_EQ("10", codec.encoder()(1));
        EXPECT_EQ(1, codec.decode("10"));
        EXPECT_EQ(1, codec.decoder()("10"));
    }

}
