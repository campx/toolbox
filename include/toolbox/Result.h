#pragma once

#include <functional>
#include <utility>

namespace toolbox
{

/** Applies a Hash function to data of type T to generate key-value pairs */
template <typename T, typename Hash = std::hash<T>>
class HashKeyValue
{
public:
    static_assert(std::is_same<typename Hash::argument_type, T>::value,
                  "Hash::argument_type != T");

    using key_type = typename Hash::result_type;
    using value_type = T;
    using pair_type = typename std::pair<key_type, T>;

    /** Construct by generating a key from the data using the given Hash */
    explicit HashKeyValue(T data, Hash hash = Hash());

    /** Component-wise constructor */
    HashKeyValue(key_type key, T data, Hash hash = Hash());

    /** Construct from a key-value pair */
    HashKeyValue(const std::pair<typename Hash::result_type, T>& pair);

    /** Set the key */
    void key(const key_type& value);

    /** Set the value and re-compute the hash key */
    void value(const T& data, bool recompute = true);

    /** Validate that the key is equal to the hash of the value */
    bool validate() const;

    /** Re-compute key value */
    void rehash();

    /** Get key associated with data */
    const key_type& key() const;

    /** Get the value */
    const value_type& value() const;

    /** Convert to a std::pair */
    explicit operator std::pair<typename Hash::result_type, T>() const;

    /** Convert to value */
    explicit operator T() const;

    bool operator==(const HashKeyValue<T, Hash>& rhs) const;

    bool operator!=(const HashKeyValue<T, Hash>& rhs) const;

private:
    pair_type pair_;
    Hash hash_;
};

template <typename T, typename Hash>
HashKeyValue<T, Hash>::HashKeyValue(T data, Hash hash) : hash_(std::move(hash))
{
    value(data);
    rehash();
}

template <typename T, typename Hash>
HashKeyValue<T, Hash>::HashKeyValue(typename Hash::result_type key,
                                    T data,
                                    Hash hash)
    : pair_(std::move(key), std::move(data)), hash_(std::move(hash))
{
}

template <typename T, typename Hash>
void HashKeyValue<T, Hash>::key(const typename Hash::result_type& value)
{
    pair_.first = value;
}

template <typename T, typename Hash>
void HashKeyValue<T, Hash>::value(const T& data, bool recompute)
{
    pair_.second = data;
    if (recompute)
    {
        rehash();
    }
}

template <typename T, typename Hash>
bool HashKeyValue<T, Hash>::validate() const
{
    return hash_(value()) == key();
}

template <typename T, typename Hash>
void HashKeyValue<T, Hash>::rehash()
{
    key(hash_(value()));
}

template <typename T, typename Hash>
const typename Hash::result_type& HashKeyValue<T, Hash>::key() const
{
    return pair_.first;
}

template <typename T, typename Hash>
const T& HashKeyValue<T, Hash>::value() const
{
    return pair_.second;
}

template <typename T, typename Hash>
HashKeyValue<T, Hash>::
operator std::pair<typename Hash::result_type, T>() const
{
    return pair_;
}

template <typename T, typename Hash>
HashKeyValue<T, Hash>::operator T() const
{
    return pair_.second;
}

template <typename T, typename Hash>
bool HashKeyValue<T, Hash>::operator==(const HashKeyValue<T, Hash>& rhs) const
{
    return key() == rhs.key();
}

template <typename T, typename Hash>
bool HashKeyValue<T, Hash>::operator!=(const HashKeyValue<T, Hash>& rhs) const
{
    return !(*this == rhs);
}

} // namespace
