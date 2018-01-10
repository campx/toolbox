#pragma once
#include <toolbox/Dereference.h>
#include <type_traits>

namespace toolbox
{

/** Holds a value which may be a raw pointer, smart pointer or stack variable
 */
template <typename T>
class Value
{
private:
    T data_;

public:
    using element_type =
        typename std::remove_reference<decltype(dereference<T>(data_))>::type;
    Value(T data);
    Value(Value&&) = default;
    Value(const Value&) = default;
    Value& operator=(Value&&) = default;
    Value& operator=(const Value&) = default;
    ~Value() = default;

    element_type* get();
    element_type& operator*();
    element_type* operator->();

    const element_type* get() const;
    const element_type& operator*() const;
    const element_type* operator->() const;
};

template <typename T>
Value<T>::Value(T data) : data_(std::move(data))
{
}

template <typename T>
typename Value<T>::element_type* Value<T>::get()
{
    return &(dereference<T>(data_));
}

template <typename T>
const typename Value<T>::element_type* Value<T>::get() const
{
    return &(dereference<const T>(data_));
}

template <typename T>
typename Value<T>::element_type* Value<T>::operator->()
{
    return get();
}

template <typename T>
const typename Value<T>::element_type* Value<T>::operator->() const
{
    return get();
}

template <typename T>
const typename Value<T>::element_type& Value<T>::operator*() const
{
    return *get();
}

template <typename T>
typename Value<T>::element_type& Value<T>::operator*()
{
    return *get();
}

} // namespace toolbox
