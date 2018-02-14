#pragma once

#include <map>
#include <toolbox/Result.h>
#include <utility>

namespace toolbox
{

/** Adapt a std::map-like class by assigning keys to values using a given
 * Hash function
 *
 * All iterators are const in order to preserve the invariant key = hash(value)
 */
template <typename T,
          typename Hash = std::hash<T>,
          typename Map = std::map<typename Hash::result_type, T>>
class HashMap
{
public:
    static_assert(std::is_same<typename Hash::result_type,
                               typename Map::key_type>::value,
                  "Hash::result_type != Map::key_type");
    static_assert(std::is_same<typename Hash::argument_type,
                               typename Map::mapped_type>::value,
                  "Hash::argument_type != Map::mapped_type");

    using key_type = typename Map::key_type;
    using mapped_type = typename Map::mapped_type;
    using value_type = typename Map::value_type;
    using size_type = typename Map::size_type;
    using difference_type = typename Map::difference_type;
    using hasher = Hash;
    using allocator_type = typename Map::allocator_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename Map::pointer;
    using const_pointer = typename Map::const_pointer;
    using iterator = typename Map::const_iterator;
    using const_iterator = typename Map::const_iterator;

    explicit HashMap(Hash hash = Hash(), Map map = Map());

    HashMap(const HashMap&) = default;

    HashMap(HashMap&&) noexcept = default;

    HashMap& operator=(const HashMap&) = default;

    HashMap& operator=(HashMap&&) noexcept = default;

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

    /** Insert an element */
    std::pair<iterator, bool> insert(const mapped_type& value);

    /** Remove an element */
    size_type erase(const key_type& key);
    size_type erase(const mapped_type& value);

    /** Find an element */
    const_iterator find(const key_type& key) const;
    const_iterator find(const mapped_type& value) const;

private:
    Hash hash_;
    Map map_;
};

template <typename T, typename Hash, typename Map>
HashMap<T, Hash, Map>::HashMap(Hash hash, Map map)
    : hash_(std::move(hash)), map_(std::move(map))
{
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::iterator HashMap<T, Hash, Map>::begin()
{
    return map_.begin();
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::iterator HashMap<T, Hash, Map>::end()
{
    return map_.end();
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::const_iterator
HashMap<T, Hash, Map>::cbegin() const
{
    return map_.cbegin();
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::const_iterator
HashMap<T, Hash, Map>::cend() const
{
    return map_.cend();
}

template <typename T, typename Hash, typename Map>
bool HashMap<T, Hash, Map>::empty() const
{
    return map_.empty();
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::size_type HashMap<T, Hash, Map>::size() const
{
    return map_.size();
}

template <typename T, typename Hash, typename Map>
void HashMap<T, Hash, Map>::clear()
{
    map_.clear();
}

template <typename T, typename Hash, typename Map>
std::pair<typename HashMap<T, Hash, Map>::iterator, bool>
HashMap<T, Hash, Map>::insert(
    const typename HashMap<T, Hash, Map>::mapped_type& value)
{
    auto key = hash_(value);
    return map_.insert(std::make_pair(key, value));
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::size_type HashMap<T, Hash, Map>::erase(
    const typename HashMap<T, Hash, Map>::key_type& key)
{
    return map_.erase(key);
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::size_type HashMap<T, Hash, Map>::erase(
    const typename HashMap<T, Hash, Map>::mapped_type& value)
{
    auto key = hash_(value);
    return map_.erase(key);
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::const_iterator HashMap<T, Hash, Map>::find(
    const typename HashMap<T, Hash, Map>::key_type& key) const
{
    return map_.find(key);
}

template <typename T, typename Hash, typename Map>
typename HashMap<T, Hash, Map>::const_iterator HashMap<T, Hash, Map>::find(
    const typename HashMap<T, Hash, Map>::mapped_type& value) const
{
    auto key = hash_(value);
    return map_.find(key);
}

} // namespace toolbox
