#pragma once
#include <map>
#include <memory>
#include <toolbox/Dereference.h>
#include <toolbox/IteratorTransformer.h>
#include <type_traits>
#include <vector>

namespace toolbox
{

/** Applies an Transform (a bijective function pair) to an AssociativeContainer
 *
 * e.g. Transform takes a std::pair<Link, Node> and calls link.id(hash(node))
 * on
 * input, passes through on output
 *
 * Need to be able to construct Link from ID
 * Set is ~ a map with comparison on first only
 *
 * Transform is a bijective pair of functors
 * Transform::first_type converts from the externally facing data type to the
 * internal data type
 * Transform::second_type converts from the internal data type to the external
 * data type
 */
template <typename Container, typename Transform>
class ContainerTransformer
{
private:
    Container container_;
    Transform transform_;

public:
    using container_type = typename std::remove_reference<decltype(
        dereference<Container>(container_))>::type;
    using value_type =
        decltype(transform_.second(typename container_type::value_type()));
    using size_type = typename container_type::size_type;
    using difference_type = typename container_type::difference_type;
    using allocator_type = typename container_type::allocator_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename container_type::pointer;
    using const_pointer = typename container_type::const_pointer;
    using iterator = IteratorTransformer<typename container_type::iterator,
                                         typename Transform::second_type>;
    using const_iterator =
        IteratorTransformer<typename container_type::const_iterator,
                            typename Transform::second_type>;

    explicit ContainerTransformer(Container container = Container(),
                                  Transform transform = Transform());

    ContainerTransformer(const ContainerTransformer&) = default;
    ContainerTransformer(ContainerTransformer&&) = default;
    ContainerTransformer& operator=(const ContainerTransformer&) = default;
    ContainerTransformer& operator=(ContainerTransformer&&) = default;

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
    std::pair<iterator, bool> insert(const value_type& value);

    /** Remove an element */
    size_type erase(
        decltype(transform_.second(typename container_type::key_type()))& key);

    /** Find an element */
    const_iterator find(const decltype(
        transform_.second(typename container_type::key_type()))& key) const;

    const container_type& container() const;
    container_type& container();
};

template <typename Container, typename Transform>
ContainerTransformer<Container, Transform>::ContainerTransformer(
    Container container, Transform transform)
    : container_(std::move(container)), transform_(std::move(transform))
{
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::iterator
ContainerTransformer<Container, Transform>::begin()
{
    return iterator(container().begin(), transform_.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::iterator
ContainerTransformer<Container, Transform>::end()
{
    return iterator(container().end(), transform_.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::const_iterator
ContainerTransformer<Container, Transform>::cbegin() const
{
    return const_iterator(container().cbegin(), transform_.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::const_iterator
ContainerTransformer<Container, Transform>::cend() const
{
    return const_iterator(container().cend(), transform_.second);
}

template <typename Container, typename Transform>
bool ContainerTransformer<Container, Transform>::empty() const
{
    return container().empty();
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::size_type
ContainerTransformer<Container, Transform>::size() const
{
    return container().size();
}

template <typename Container, typename Transform>
void ContainerTransformer<Container, Transform>::clear()
{
    container().clear();
}

template <typename Container, typename Transform>
std::pair<typename ContainerTransformer<Container, Transform>::iterator, bool>
ContainerTransformer<Container, Transform>::insert(
    const typename ContainerTransformer<Container, Transform>::value_type&
        value)
{
    auto result = container().insert(transform_.first(value));
    return std::make_pair(iterator(result.first), result.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::size_type
ContainerTransformer<Container, Transform>::erase(
    decltype(transform_.second(typename container_type::key_type()))& key)
{
    return container().erase(transform_.first(key));
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::const_iterator
ContainerTransformer<Container, Transform>::find(const decltype(
    transform_.second(typename container_type::key_type()))& key) const
{
    return const_iterator(container().find(transform_.first(key)));
}

template <typename Container, typename Transform>
const typename ContainerTransformer<Container, Transform>::container_type&
ContainerTransformer<Container, Transform>::container() const
{
    return dereference<const Container>(container_);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::container_type&
ContainerTransformer<Container, Transform>::container()
{
    return dereference<Container>(container_);
}

} // namespace toolbox
