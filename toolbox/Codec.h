#pragma once

#include <utility>

template <typename Encoder, typename Decoder>
class Codec
{
public:
    using encoder_type = Encoder;
    using decoder_type = Decoder;

    Codec(Encoder encoder, Decoder decoder);

    const Encoder& encoder() const;

    const Decoder& decoder() const;

    auto encode(const auto& input);

    auto decode(const auto& input);

private:
    Encoder encoder_;
    Decoder decoder_;
};

template <typename Encoder, typename Decoder>
Codec<Encoder, Decoder>::Codec(Encoder encoder, Decoder decoder) : encoder_(
    std::move(encoder)), decoder_(std::move(decoder))
{
}

template <typename Encoder, typename Decoder>
auto Codec<Encoder, Decoder>::decode(const auto& input)
{
    return decoder_(input);
}

template <typename Encoder, typename Decoder>
auto Codec<Encoder, Decoder>::encode(const auto& input)
{
    return encoder_(input);
}

template <typename Encoder, typename Decoder>
const Encoder& Codec<Encoder, Decoder>::encoder() const
{
    return encoder_;
};

template <typename Encoder, typename Decoder>
const Decoder& Codec<Encoder, Decoder>::decoder() const
{
    return decoder_;
}

template <typename Encoder, typename Decoder>
Codec<Encoder, Decoder> makeCodec(Encoder encoder, Decoder decoder)
{
    return Codec<Encoder, Decoder>(encoder, decoder);
};
