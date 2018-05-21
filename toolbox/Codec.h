#pragma once

#include <utility>

template <typename Encoder, typename Decoder>
class Codec
{

private:
    Encoder encoder_;
    Decoder decoder_;

public:
    using encoder_type = Encoder;
    using decoder_type = Decoder;

    Codec(Encoder encoder = Encoder(), Decoder decoder = Decoder());

    Encoder& encoder();

    Decoder& decoder();

    template <typename Input>
    auto encode(const Input& input) -> decltype(encoder_(input))
    {
        return encoder_(input);
    }

    template <typename Input>
    auto decode(const Input& input) -> decltype(decoder_(input))
    {
        return decoder_(input);
    }
};

template <typename Encoder, typename Decoder>
Codec<Encoder, Decoder>::Codec(Encoder encoder, Decoder decoder)
    : encoder_(std::move(encoder)), decoder_(std::move(decoder))
{
}

template <typename Encoder, typename Decoder>
Encoder& Codec<Encoder, Decoder>::encoder()
{
    return encoder_;
};

template <typename Encoder, typename Decoder>
Decoder& Codec<Encoder, Decoder>::decoder()
{
    return decoder_;
}

template <typename Encoder, typename Decoder>
Codec<Encoder, Decoder> makeCodec(Encoder encoder, Decoder decoder)
{
    return Codec<Encoder, Decoder>(encoder, decoder);
};
