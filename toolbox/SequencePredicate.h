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
template <typename Sequence,
          typename Predicate = std::equal_to<typename Sequence::value_type>>
class SequencePredicate
{
public: /** Type definitions */
    using value_type = typename Sequence::value_type;
    using const_iterator = typename Sequence::const_iterator;
    using argument_type = std::pair<value_type, value_type>;
    using result_type = bool;

public: /** Constructors */
    explicit SequencePredicate(Sequence sequence = Sequence(),
                               Predicate predicate = Predicate());

public: /** Operators */
    result_type operator()(const argument_type& input);

private:                           /** Data */
    Sequence sequence_;            /**< Sequence of data to compare against */
    Predicate predicate_;          /**< Predicate to match values */
    const_iterator current_value_; /**< Current value in sequence */
    const_iterator next_value_;    /**< Next value in sequence */
};

template <typename Sequence, typename Predicate>
SequencePredicate<Sequence, Predicate>
makeSequencePredicate(Sequence sequence, Predicate predicate);

template <typename Sequence, typename Predicate>
SequencePredicate<Sequence, Predicate>::SequencePredicate(Sequence sequence,
                                                          Predicate predicate)
    : sequence_(std::move(sequence)), predicate_(std::move(predicate)),
      current_value_(sequence_.begin()),
      next_value_(sequence_.empty() ? current_value_ : ++sequence_.begin())
{
}

template <typename Sequence, typename Predicate>
SequencePredicate<Sequence, Predicate>
makeSequencePredicate(Sequence sequence, Predicate predicate)
{
    return SequencePredicate<decltype(sequence), decltype(predicate)>(
        std::forward<Sequence>(sequence), std::forward<Predicate>(predicate));
}

template <typename Sequence, typename Predicate>
typename SequencePredicate<Sequence, Predicate>::result_type
SequencePredicate<Sequence, Predicate>::
operator()(const typename SequencePredicate<Sequence,
                                            Predicate>::argument_type& input)
{
    auto result = next_value_ != sequence_.end() &&
                  predicate_(input.first, *current_value_) &&
                  predicate_(input.second, *next_value_);
    if (result)
    {
        current_value_++;
        next_value_++;
    }
    return result;
}

} // namespace toolbox
