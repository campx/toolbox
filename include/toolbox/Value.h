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
public:
    using element_type =
        typename std::remove_reference<decltype(dereference<T>(T()))>::type;
    Value(T data);
    const element_type& get() const;
    element_type& get();

private:
    T data_;
};

template <typename T>
Value<T>::Value(T data) : data_(std::move(data))
{
}

template <typename T>
const typename Value<T>::element_type& Value<T>::get() const
{
    return dereference<const T>(data_);
}

template <typename T>
typename Value<T>::element_type& Value<T>::get()
{
    return dereference<T>(data_);
}

} // namespace toolbox
