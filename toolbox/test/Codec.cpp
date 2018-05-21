#include "gtest/gtest.h"
#include <toolbox/Codec.h>

struct Encoder
{
    std::string operator()(int value)
    {
        return std::to_string(value * 10);
    }
};

struct Decoder
{
    int operator()(const std::string& value)
    {
        return std::atoi(value.c_str()) / 10;
    }
};

TEST(Toolbox, Codec)
{
    {
        auto codec = Codec<Encoder, Decoder>();
        EXPECT_EQ(std::string("10"), codec.encode(1));
        EXPECT_EQ("10", codec.encoder()(1));
        EXPECT_EQ(1, codec.decode(std::string("10")));
        EXPECT_EQ(1, codec.decoder()(std::string("10")));
    }
    {
        auto codec = makeCodec([](int v) { return std::to_string(v * 10); },
                               [](const std::string& s) {
                                   return std::atoi(s.c_str()) / 10;
                               });
        EXPECT_EQ("10", codec.encode(1));
        EXPECT_EQ("10", codec.encoder()(1));
        EXPECT_EQ(1, codec.decode(std::string("10")));
        EXPECT_EQ(1, codec.decoder()(std::string("10")));
    }
}
