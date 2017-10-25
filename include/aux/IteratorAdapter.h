#pragma once
#include <memory>

namespace aux
{

template <typename Iterator, typename Function>
class IteratorAdapter : public std::iterator<std::input_iterator_tag,
                                             typename Function::result_type>
{
public:
    using self_type = IteratorAdapter;

    explicit IteratorAdapter(Iterator it, Function function = Function{});
    IteratorAdapter(const IteratorAdapter& value) = default;
    ~IteratorAdapter() = default;
    IteratorAdapter(IteratorAdapter&&) = default;
    IteratorAdapter& operator=(const IteratorAdapter&) = default;
    IteratorAdapter& operator=(IteratorAdapter&&) = default;

    Iterator iterator() const;

    self_type operator++();
    self_type operator++(int dummy);
    typename Function::result_type& operator*();
    typename Function::result_type* operator->();

    bool operator==(const IteratorAdapter& rhs) const;
    bool operator!=(const IteratorAdapter& rhs) const;

private:
    Iterator it_;
    Function function_;
    typename Function::result_type value_;
    bool dirty_flag_;

    void evaluate();
    void increment();
};

template <typename Iterator, typename Function>
IteratorAdapter<Iterator, Function>::IteratorAdapter(Iterator it,
                                                     Function function)
    : it_(std::move(it)), function_(function), dirty_flag_(true)
{
}

template <typename Iterator, typename Function>
Iterator IteratorAdapter<Iterator, Function>::iterator() const
{
    return it_;
}

template <typename Iterator, typename Function>
void IteratorAdapter<Iterator, Function>::increment()
{
    ++it_;
    dirty_flag_ = true;
}

template <typename Iterator, typename Function>
typename IteratorAdapter<Iterator, Function>::self_type
    IteratorAdapter<Iterator, Function>::operator++()
{
    increment();
    return *this;
}

template <typename Iterator, typename Function>
typename IteratorAdapter<Iterator, Function>::self_type
    IteratorAdapter<Iterator, Function>::operator++(int dummy)
{
    dummy++;
    auto tmp = *this;
    increment();
    return tmp;
}

template <typename Iterator, typename Function>
void IteratorAdapter<Iterator, Function>::evaluate()
{
    if (dirty_flag_)
    {
        value_ = function_(*it_);
        dirty_flag_ = false;
    }
}

template <typename Iterator, typename Function>
typename Function::result_type& IteratorAdapter<Iterator, Function>::
operator*()
{
    evaluate();
    return value_;
}

template <typename Iterator, typename Function>
typename Function::result_type* IteratorAdapter<Iterator, Function>::
operator->()
{
    evaluate();
    return &value_;
}

template <typename Iterator, typename Function>
bool IteratorAdapter<Iterator, Function>::
operator==(const IteratorAdapter<Iterator, Function>& rhs) const
{
    return it_ == rhs.it_;
}

template <typename Iterator, typename Function>
bool IteratorAdapter<Iterator, Function>::
operator!=(const IteratorAdapter<Iterator, Function>& rhs) const
{
    return it_ != rhs.it_;
}

} // namespace aux
