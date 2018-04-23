#pragma once
#include <functional>
#include <utility>

namespace toolbox
{

/** Applies a comparison to pairs of values to those within a given sequence
 *
 * Whenever a match occurs, position markers within the sequence advance
 *
 * Example:
 * sequence = "/", "home", "/", "user"
 * input = ("home", "/")
 * result = true
 * */
template <typename Iterator,
          typename Compare = std::equal_to<decltype(*Iterator())>>
class SequencePredicate
{
public: /** Type definitions */
    using value_type = decltype(*Iterator());
    using argument_type = std::pair<value_type, value_type>;
    using result_type = bool;

public: /** Constructors */
    explicit SequencePredicate(Iterator begin = Iterator(),
                               Iterator end = Iterator(),
                               Compare compare = Compare());

public: /** Operators */
    result_type operator()(const argument_type& input);

private:               /** Data */
    Iterator begin_;   /**< Beginning of sequence */
    Iterator end_;     /**< End of sequence */
    Compare compare_;  /**< Compare to match values */
    Iterator current_; /**< Current value in sequence */
    Iterator next_;    /**< Next value in sequence */
};

template <typename Iterator, typename Compare>
SequencePredicate<Iterator, Compare>
makeSequencePredicate(Iterator begin, Iterator end, Compare compare);

template <typename Iterator, typename Compare>
SequencePredicate<Iterator, Compare>::SequencePredicate(Iterator begin,
                                                        Iterator end,
                                                        Compare compare)
    : begin_(std::move(begin)), end_(std::move(end)),
      compare_(std::move(compare)), current_(begin_), next_(current_)
{
    if (begin_ != end_)
    {
        ++next_;
    }
}

template <typename Iterator, typename Compare>
SequencePredicate<Iterator, Compare>
makeSequencePredicate(Iterator begin, Iterator end, Compare compare)
{
    return SequencePredicate<decltype(begin), decltype(compare)>(
        std::forward<Iterator>(begin), std::forward<Iterator>(end),
        std::forward<Compare>(compare));
}

template <typename Iterator, typename Compare>
typename SequencePredicate<Iterator, Compare>::result_type
SequencePredicate<Iterator, Compare>::operator()(
    const typename SequencePredicate<Iterator, Compare>::argument_type& input)
{
    auto result = next_ != end_ && compare_(input.first, *current_) &&
                  compare_(input.second, *next_);
    if (result)
    {
        current_++;
        next_++;
    }
    return result;
}

} // namespace toolbox
