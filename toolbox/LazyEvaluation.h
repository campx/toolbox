#pragma once

#include <functional>
#include <utility>

namespace toolbox
{

/** Holds the result of invoking a Callable on a given input argument
 * Stores a std::pair<S, T> such that S = f(T) where F is the Callable */
template <typename T, typename Callable>
class LazyEvaluation
{
public:
    using argument_type = typename std::remove_const<T>::type;
    using result_type = decltype(Callable()(T()));

    /** Component-wise constructor */
    explicit LazyEvaluation(T argument = T(),
                            Callable f = Callable(),
                            bool dirty = true,
                            result_type result = result_type());

    /** Set the result */
    LazyEvaluation& result(const result_type& value);

    /** Set the argument
     **@param data  Value to which data will be set
     **@param dirty If true then do not recompute the callable */
    LazyEvaluation& argument(const T& value);

    /** Check whether the dirty flag is set, meaning a recompute is required */
    bool dirty() const;

    /** Forcibly set the dirty flag value */
    LazyEvaluation& dirty(bool value);

    /** Validate that the result is equal to the callable of the argument */
    bool validate() const;

    /** Compute the result */
    result_type compute() const;

    /** Get result associated with data without recomputing */
    const result_type& result() const;

    /** Lazily recompute and get the result */
    const result_type& get();

    /** Get the argument */
    const argument_type& argument() const;

    bool operator==(const LazyEvaluation<T, Callable>& rhs) const;

    bool operator!=(const LazyEvaluation<T, Callable>& rhs) const;

private:
    result_type result_;
    argument_type argument_;
    Callable f_;
    bool dirty_;
};

template <typename T, typename Callable>
LazyEvaluation<T, Callable>::LazyEvaluation(
    T argument,
    Callable f,
    bool dirty,
    typename LazyEvaluation<T, Callable>::result_type result)
    : result_(std::move(result)), argument_(std::move(argument)),
      f_(std::move(f)), dirty_(dirty)
{
}

template <typename T, typename Callable>
LazyEvaluation<T, Callable>& LazyEvaluation<T, Callable>::result(
    const typename LazyEvaluation<T, Callable>::result_type& value)
{
    result_ = value;
    dirty(false);
    return *this;
}

template <typename T, typename Callable>
const typename LazyEvaluation<T, Callable>::result_type&
LazyEvaluation<T, Callable>::get()
{
    if (dirty())
    {
        result(compute());
    }
    return result();
}

template <typename T, typename Callable>
LazyEvaluation<T, Callable>&
LazyEvaluation<T, Callable>::argument(const T& value)
{
    argument_ = value;
    dirty(true);
    return *this;
}

template <typename T, typename Callable>
bool LazyEvaluation<T, Callable>::dirty() const
{
    return dirty_;
}

template <typename T, typename Callable>
LazyEvaluation<T, Callable>& LazyEvaluation<T, Callable>::dirty(bool value)
{
    dirty_ = value;
    return *this;
}

template <typename T, typename Callable>
bool LazyEvaluation<T, Callable>::validate() const
{
    return compute() == result();
}

template <typename T, typename Callable>
typename LazyEvaluation<T, Callable>::result_type
LazyEvaluation<T, Callable>::compute() const
{
    return f_(argument());
}

template <typename T, typename Callable>
const typename LazyEvaluation<T, Callable>::result_type&
LazyEvaluation<T, Callable>::result() const
{
    return result_;
}

template <typename T, typename Callable>
const typename LazyEvaluation<T, Callable>::argument_type&
LazyEvaluation<T, Callable>::argument() const
{
    return argument_;
}

template <typename T, typename Callable>
bool LazyEvaluation<T, Callable>::
operator==(const LazyEvaluation<T, Callable>& rhs) const
{
    return result() == rhs.result();
}

template <typename T, typename Callable>
bool LazyEvaluation<T, Callable>::
operator!=(const LazyEvaluation<T, Callable>& rhs) const
{
    return !(*this == rhs);
}

} // namespace
