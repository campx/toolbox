#pragma once
#include <memory>
#include <type_traits>

namespace toolbox
{

namespace detail
{
template <typename T>
struct is_smart_pointer_helper : std::false_type
{
};

template <typename T>
struct is_smart_pointer_helper<std::shared_ptr<T>> : std::true_type
{
};

template <typename T>
struct is_smart_pointer_helper<std::unique_ptr<T>> : std::true_type
{
};

template <typename T>
struct is_smart_pointer_helper<std::weak_ptr<T>> : std::true_type
{
};

} // namespace detail

template <typename T>
struct is_smart_pointer
    : detail::is_smart_pointer_helper<typename std::remove_cv<T>::type>
{
};

/** Dereference a smart pointer */
template <typename T>
typename std::enable_if<is_smart_pointer<T>::value,
                        typename T::element_type>::type&
dereference(T& value)
{
    return *value;
}

/** Dereference a raw pointer */
template <typename T>
typename std::enable_if<std::is_pointer<T>::value,
                        std::remove_pointer<T>>::type&
dereference(T& value)
{
    return *value;
}

/** Dereference a value which isn't a smart pointer or a raw pointer */
template <typename T>
typename std::enable_if<!std::is_pointer<T>::value &&
                            !is_smart_pointer<T>::value,
                        T>::type&
dereference(T& value)
{
    return value;
}

} // namespace toolbox
