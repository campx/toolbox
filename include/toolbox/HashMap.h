#pragma once
#include <utility>

namespace aux
{

template <typename Hash, typename Map>
class HashMap
{
public:
    static_assert(std::is_same<typename Hash::result_type,
                               typename Map::key_type>::value,
                  "Hash::result_type != Map::key_type");
    static_assert(std::is_same<typename Hash::argument_type,
                               typename Map::mapped_type>::value,
                  "Hash::argument_type != Map::mapped_type");

    using key_type = typename Hash::result_type;
    using mapped_type = typename Hash::argument_type;
    using value_type = std::pair<const key_type, mapped_type>;

    using size_type = typename Map::size_type;
    using difference_type = typename Map::difference_type;
    using hasher = Hash;
    using allocator_type = typename Map::allocator_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename Map::pointer;
    using const_pointer = typename Map::const_pointer;
    using iterator = typename Map::iterator;
    using const_iterator = typename Map::const_iterator;

    explicit HashMap(Hash hash = Hash(), Map store = Map());
    HashMap(const HashMap&) = default;
    HashMap(HashMap&&) = default;
    HashMap& operator=(const HashMap&) = default;
    HashMap& operator=(HashMap&&) = default;

    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;

    /** Determine whether the container is empty */
    bool empty() const;
    /** Get the number of values stored by the container */
    size_type size() const;
    /** Remove all values from the container */
    void clear();

    /** Insert a value.
     *
     * A value_type is inserted to store using the key generate by hasher */
    std::pair<iterator, bool> insert(const mapped_type& value);

    /** Remove the element identified by the given key */
    size_type erase(const key_type& key);

    /** Find the element identified by the given key */
    iterator find(const key_type& key);

private:
    Hash hash_;
    Map map_;
};

template <typename Hash, typename Map>
HashMap<Hash, Map>::HashMap(Hash hash, Map store)
    : hash_(std::move(hash)), map_(std::move(store))
{
}

template <typename Hash, typename Map>
typename HashMap<Hash, Map>::iterator HashMap<Hash, Map>::begin()
{
    return map_.begin();
}

template <typename Hash, typename Map>
typename HashMap<Hash, Map>::iterator HashMap<Hash, Map>::end()
{
    return map_.end();
}

template <typename Hash, typename Map>
typename HashMap<Hash, Map>::const_iterator HashMap<Hash, Map>::cbegin() const
{
    return map_.cbegin();
}

template <typename Hash, typename Map>
typename HashMap<Hash, Map>::const_iterator HashMap<Hash, Map>::cend() const
{
    return map_.cend();
}

template <typename Hash, typename Map>
std::pair<typename HashMap<Hash, Map>::iterator, bool>
HashMap<Hash, Map>::insert(
    const typename HashMap<Hash, Map>::mapped_type& value)
{
    auto key = hash_(value);
    return map_.insert(std::make_pair(key, value));
}

template <typename Hash, typename Map>
typename HashMap<Hash, Map>::size_type
HashMap<Hash, Map>::erase(const typename HashMap<Hash, Map>::key_type& key)
{
    return map_.erase(key);
}

template <typename Hash, typename Map>
typename HashMap<Hash, Map>::iterator
HashMap<Hash, Map>::find(const typename HashMap<Hash, Map>::key_type& key)
{
    return map_.find(key);
}

} // namespace aux
