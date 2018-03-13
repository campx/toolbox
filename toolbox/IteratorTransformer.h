#pragma once
#include <memory>

namespace toolbox
{

template <typename Iterator, typename Transform>
class IteratorTransformer
    : public std::iterator<std::input_iterator_tag,
                           decltype(
                               Transform()(typename Iterator::value_type()))>
{
public:
    using self_type = IteratorTransformer;

    explicit IteratorTransformer(Iterator it,
                                 Transform transform = Transform{});
    IteratorTransformer(const IteratorTransformer& value) = default;
    ~IteratorTransformer() = default;
    IteratorTransformer(IteratorTransformer&&) = default;
    IteratorTransformer& operator=(const IteratorTransformer&) = default;
    IteratorTransformer& operator=(IteratorTransformer&&) = default;

    Iterator iterator() const;

    using value_type = decltype(Transform()(typename Iterator::value_type()));
    using reference_type = value_type&;
    using pointer_type = value_type*;

    self_type operator++();
    self_type operator++(int dummy);
    reference_type operator*();
    pointer_type operator->();

    bool operator==(const IteratorTransformer& rhs) const;
    bool operator!=(const IteratorTransformer& rhs) const;

private:
    Iterator it_;
    Transform transform_;
    value_type value_;
    bool dirty_flag_;

    void evaluate();
    void increment();
};

template <typename Iterator, typename Transform>
IteratorTransformer<Iterator, Transform>::IteratorTransformer(
    Iterator it, Transform transform)
    : it_(std::move(it)), transform_(transform), dirty_flag_(true)
{
}

template <typename Iterator, typename Transform>
Iterator IteratorTransformer<Iterator, Transform>::iterator() const
{
    return it_;
}

template <typename Iterator, typename Transform>
void IteratorTransformer<Iterator, Transform>::increment()
{
    ++it_;
    dirty_flag_ = true;
}

template <typename Iterator, typename Transform>
typename IteratorTransformer<Iterator, Transform>::self_type
    IteratorTransformer<Iterator, Transform>::operator++()
{
    increment();
    return *this;
}

template <typename Iterator, typename Transform>
typename IteratorTransformer<Iterator, Transform>::self_type
    IteratorTransformer<Iterator, Transform>::operator++(int dummy)
{
    dummy++;
    auto tmp = *this;
    increment();
    return tmp;
}

template <typename Iterator, typename Transform>
void IteratorTransformer<Iterator, Transform>::evaluate()
{
    if (dirty_flag_)
    {
        value_ = transform_(*it_);
        dirty_flag_ = false;
    }
}

template <typename Iterator, typename Transform>
typename IteratorTransformer<Iterator, Transform>::reference_type
    IteratorTransformer<Iterator, Transform>::operator*()
{
    evaluate();
    return value_;
}

template <typename Iterator, typename Transform>
typename IteratorTransformer<Iterator, Transform>::pointer_type
    IteratorTransformer<Iterator, Transform>::operator->()
{
    evaluate();
    return &value_;
}

template <typename Iterator, typename Transform>
bool IteratorTransformer<Iterator, Transform>::
operator==(const IteratorTransformer<Iterator, Transform>& rhs) const
{
    return it_ == rhs.it_;
}

template <typename Iterator, typename Transform>
bool IteratorTransformer<Iterator, Transform>::
operator!=(const IteratorTransformer<Iterator, Transform>& rhs) const
{
    return it_ != rhs.it_;
}

} // namespace toolbox
