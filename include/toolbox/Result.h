#pragma once

#include <functional>
#include <utility>

namespace toolbox
{

/** Holds the result of invoking a Callable on a given input argument
 * Stores a std::pair<S, T> such that S = f(T) where F is the Callable */
template <typename T, typename Callable = std::hash<T>>
class Result
{
public:
    using argument_type = T;
    using result_type = decltype(Callable()(T()));
    using pair_type = typename std::pair<result_type, T>;

    /** Construct by generating a result from the data using the given
     * Callable */
    explicit Result(T value, Callable f = Callable());

    /** Component-wise constructor */
    Result(result_type result,
           T argument,
           Callable f = Callable(),
           bool dirty = false);

    /** Construct from a result-argument pair */
    Result(const std::pair<result_type, T>& pair);

    /** Set the result */
    Result& result(const result_type& value);

    /** Set the argument
     **@param data  Value to which data will be set
     **@param dirty If true then do not recompute the callable */
    Result& argument(const T& value);

    /** Check whether the dirty flag is set, meaning a recompute is required */
    bool dirty() const;

    /** Forcibly set the dirty flag value */
    Result& dirty(bool value);

    /** Validate that the result is equal to the callable of the argument */
    bool validate() const;

    /** Compute the result */
    result_type compute() const;

    /** Get result associated with data */
    const result_type& result() const;

    /** Lazily recompute and get the result */
    const result_type& get();

    /** Get the argument */
    const argument_type& argument() const;

    /** Convert to a std::pair */
    explicit operator std::pair<typename Callable::result_type, T>() const;

    /** Convert to argument */
    explicit operator T() const;

    bool operator==(const Result<T, Callable>& rhs) const;

    bool operator!=(const Result<T, Callable>& rhs) const;

private:
    pair_type pair_;
    Callable f_;
    bool dirty_;
};

template <typename T, typename Callable>
Result<T, Callable>::Result(T value, Callable f)
    : f_(std::move(f)), dirty_(true)
{
    argument(value);
}

template <typename T, typename Callable>
Result<T, Callable>::Result(typename Result<T, Callable>::result_type result,
                            T value,
                            Callable f,
                            bool dirty)
    : pair_(std::move(result), std::move(value)), f_(std::move(f)),
      dirty_(dirty)
{
}

template <typename T, typename Callable>
Result<T, Callable>& Result<T, Callable>::result(
    const typename Result<T, Callable>::result_type& value)
{
    pair_.first = value;
    dirty(false);
    return *this;
}

template <typename T, typename Callable>
const typename Result<T, Callable>::result_type& Result<T, Callable>::get()
{
    if (dirty())
    {
        result(compute());
    }
    return result();
}

template <typename T, typename Callable>
Result<T, Callable>& Result<T, Callable>::argument(const T& value)
{
    pair_.second = value;
    dirty(true);
    return *this;
}

template <typename T, typename Callable>
bool Result<T, Callable>::dirty() const
{
    return dirty_;
}

template <typename T, typename Callable>
Result<T, Callable>& Result<T, Callable>::dirty(bool value)
{
    dirty_ = value;
    return *this;
}

template <typename T, typename Callable>
bool Result<T, Callable>::validate() const
{
    return compute() == result();
}

template <typename T, typename Callable>
typename Result<T, Callable>::result_type Result<T, Callable>::compute() const
{
    return f_(argument());
}

template <typename T, typename Callable>
const typename Result<T, Callable>::result_type&
Result<T, Callable>::result() const
{
    return pair_.first;
}

template <typename T, typename Callable>
const T& Result<T, Callable>::argument() const
{
    return pair_.second;
}

template <typename T, typename Callable>
Result<T, Callable>::
operator std::pair<typename Callable::result_type, T>() const
{
    return pair_;
}

template <typename T, typename Callable>
Result<T, Callable>::operator T() const
{
    return pair_.second;
}

template <typename T, typename Callable>
bool Result<T, Callable>::operator==(const Result<T, Callable>& rhs) const
{
    return result() == rhs.result();
}

template <typename T, typename Callable>
bool Result<T, Callable>::operator!=(const Result<T, Callable>& rhs) const
{
    return !(*this == rhs);
}

} // namespace
