#pragma once
#include <memory>
#include <toolbox/IteratorTransformer.h>
#include <toolbox/Value.h>
#include <type_traits>

namespace toolbox
{

/** Applies a Transform to an AssociativeContainer
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
    Value<Container> container_;
    Transform transform_;

public:
    using container_type = typename Value<Container>::element_type;
    using value_type =
        decltype(transform_.second(typename container_type::value_type()));
    using size_type = typename container_type::size_type;
    using difference_type = typename container_type::difference_type;
    using allocator_type = typename container_type::allocator_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename container_type::pointer;
    using const_pointer = typename container_type::const_pointer;
    using iterator = IteratorTransformer<typename Transform::second_type,
                                         typename container_type::iterator>;
    using const_iterator =
        IteratorTransformer<typename Transform::second_type,
                            typename container_type::const_iterator>;

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

    const container_type& container() const;

    container_type& container();

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
    return iterator(container_->begin(), transform_.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::iterator
ContainerTransformer<Container, Transform>::end()
{
    return iterator(container_->end(), transform_.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::const_iterator
ContainerTransformer<Container, Transform>::cbegin() const
{
    return const_iterator(container_->cbegin(), transform_.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::const_iterator
ContainerTransformer<Container, Transform>::cend() const
{
    return const_iterator(container_->cend(), transform_.second);
}

template <typename Container, typename Transform>
const typename ContainerTransformer<Container, Transform>::container_type&
ContainerTransformer<Container, Transform>::container() const
{
    return *container_;
};

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::container_type&
ContainerTransformer<Container, Transform>::container()
{
    return *container_;
};

template <typename Container, typename Transform>
bool ContainerTransformer<Container, Transform>::empty() const
{
    return container_->empty();
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::size_type
ContainerTransformer<Container, Transform>::size() const
{
    return container_->size();
}

template <typename Container, typename Transform>
void ContainerTransformer<Container, Transform>::clear()
{
    container_->clear();
}

template <typename Container, typename Transform>
std::pair<typename ContainerTransformer<Container, Transform>::iterator, bool>
ContainerTransformer<Container, Transform>::insert(
    const typename ContainerTransformer<Container, Transform>::value_type&
        value)
{
    auto result = container_->insert(transform_.first(value));
    return std::make_pair(iterator(result.first), result.second);
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::size_type
ContainerTransformer<Container, Transform>::erase(
    decltype(transform_.second(typename container_type::key_type()))& key)
{
    return container_->erase(transform_.first(key));
}

template <typename Container, typename Transform>
typename ContainerTransformer<Container, Transform>::const_iterator
ContainerTransformer<Container, Transform>::find(const decltype(
    transform_.second(typename container_type::key_type()))& key) const
{
    return const_iterator(container_->find(transform_.first(key)));
}

} // namespace toolbox
