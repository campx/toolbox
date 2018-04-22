#pragma once
#include <functional>
#include <utility>

namespace toolbox
{

/** Applies a predicate to pairs of values to those within a given sequence
 *
 * Whenever a match occurs, position markers within the sequence advance
 *
 * Example:
 * sequence = "/", "home", "/", "user"
 * input = ("home", "/")
 * result = true
 * */
template <typename Iterator,
          typename Predicate = std::equal_to<decltype(*Iterator())>>
class SequencePredicate
{
public: /** Type definitions */
    using value_type = decltype(*Iterator());
    using argument_type = std::pair<value_type, value_type>;
    using result_type = bool;

public: /** Constructors */
    explicit SequencePredicate(Iterator begin = Iterator(),
                               Iterator end = Iterator(),
                               Predicate predicate = Predicate());

public: /** Operators */
    result_type operator()(const argument_type& input);

private:                  /** Data */
    Iterator begin_;      /**< Beginning of sequence */
    Iterator end_;        /**< End of sequence */
    Predicate predicate_; /**< Predicate to match values */
    Iterator current_;    /**< Current value in sequence */
    Iterator next_;       /**< Next value in sequence */
};

template <typename Iterator, typename Predicate>
SequencePredicate<Iterator, Predicate>
makeSequencePredicate(Iterator begin, Iterator end, Predicate predicate);

template <typename Iterator, typename Predicate>
SequencePredicate<Iterator, Predicate>::SequencePredicate(Iterator begin,
                                                          Iterator end,
                                                          Predicate predicate)
    : begin_(std::move(begin)), end_(std::move(end)),
      predicate_(std::move(predicate)), current_(begin_), next_(current_)
{
    if (begin_ != end_)
    {
        ++next_;
    }
}

template <typename Iterator, typename Predicate>
SequencePredicate<Iterator, Predicate>
makeSequencePredicate(Iterator begin, Iterator end, Predicate predicate)
{
    return SequencePredicate<decltype(begin), decltype(predicate)>(
        std::forward<Iterator>(begin), std::forward<Iterator>(end),
        std::forward<Predicate>(predicate));
}

template <typename Iterator, typename Predicate>
typename SequencePredicate<Iterator, Predicate>::result_type
SequencePredicate<Iterator, Predicate>::
operator()(const typename SequencePredicate<Iterator,
                                            Predicate>::argument_type& input)
{
    auto result = next_ != end_ && predicate_(input.first, *current_) &&
                  predicate_(input.second, *next_);
    if (result)
    {
        current_++;
        next_++;
    }
    return result;
}

} // namespace toolbox
