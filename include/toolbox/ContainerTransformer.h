#pragma once
#include <aux/IteratorAdapter.h>
#include <memory>
#include <type_traits>

namespace aux
{

/** The identity bijunction; simply returns its input */
template <typename T>
struct Identity
{
    using first_type = std::function<T(T)>;
    using second_type = std::function<T(T)>;
    first_type first;
    second_type second;
    static T f(const T& input);
    Identity();
};

/** KeyBijection and ValueBijection are each a std::pair of symmetric functions
 *
 * Choosing to pair functions by type rather than input/output results in no
 * loss of generality, since the bijections can be appropriated elsewhere, for
 * example in encoding/decoding each type for network messaging, or for
 * implementing FunctionalSet etc
 */
template <typename Map, typename KeyBijection, typename ValueBijection>
class MapAdapter
{
public:
    // Ensure the functions are reciprocal
    using KeyInputFunction = typename KeyBijection::first_type;
    using KeyOutputFunction = typename KeyBijection::second_type;
    using ValueInputFunction = typename ValueBijection::first_type;
    using ValueOutputFunction = typename ValueBijection::second_type;

    using storage_value_type = typename Map::value_type;
    using key_type = typename KeyBijection::second_type::result_type;
    using mapped_type = typename ValueBijection::second_type::result_type;
    using value_type = std::pair<key_type, mapped_type>;

    struct InputFunction
    {
        using argument_type = value_type;
        using result_type = storage_value_type;
        InputFunction(KeyInputFunction kf, ValueInputFunction vf);
        result_type operator()(const argument_type& input) const;

        KeyInputFunction first;
        ValueInputFunction second;
    };

    struct OutputFunction
    {
        using argument_type = storage_value_type;
        using result_type = value_type;
        OutputFunction(KeyOutputFunction kf, ValueOutputFunction vf);
        result_type operator()(const argument_type& input) const;

        KeyOutputFunction first;
        ValueOutputFunction second;
    };

    using iterator = ForwardIterator<typename Map::iterator, OutputFunction>;
    using const_iterator = iterator;
    using size_type = typename Map::size_type;

    explicit MapAdapter(std::shared_ptr<Map> storage,
                        KeyBijection key_function = KeyBijection(),
                        ValueBijection value_function = ValueBijection());

    iterator begin();
    iterator end();

    size_type size() const;

    std::pair<iterator, bool> insert(const value_type& value);
    const_iterator find(const key_type& key) const;
    size_type erase(const key_type& key);

private:
    std::shared_ptr<Map> storage_;
    InputFunction input_;
    OutputFunction output_;
};

/*******************************IMPLEMENTATION*****************************/

template <typename T>
T Identity<T>::f(const T& input)
{
    return input;
}

template <typename T>
Identity<T>::Identity() : first(&f), second(&f)
{
}

template <typename Map, typename KeyBijection, typename ValueBijection>
MapAdapter<Map, KeyBijection, ValueBijection>::InputFunction::InputFunction(
    typename KeyBijection::first_type kf,
    typename ValueBijection::first_type vf)
    : first(std::move(kf)), second(std::move(vf))
{
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename Map::value_type
MapAdapter<Map, KeyBijection, ValueBijection>::InputFunction::operator()(
    const typename MapAdapter<Map, KeyBijection, ValueBijection>::value_type&
        input) const
{
    return std::make_pair(first(input.first), second(input.second));
}

template <typename Map, typename KeyBijection, typename ValueBijection>
MapAdapter<Map, KeyBijection, ValueBijection>::OutputFunction::OutputFunction(
    typename KeyBijection::second_type kf,
    typename ValueBijection::second_type vf)
    : first(std::move(kf)), second(std::move(vf))
{
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename MapAdapter<Map, KeyBijection, ValueBijection>::value_type
MapAdapter<Map, KeyBijection, ValueBijection>::OutputFunction::
operator()(const typename Map::value_type& input) const
{
    return std::make_pair(first(input.first), second(input.second));
}

template <typename Map, typename KeyBijection, typename ValueBijection>
MapAdapter<Map, KeyBijection, ValueBijection>::MapAdapter(
    std::shared_ptr<Map> storage,
    KeyBijection key_function,
    ValueBijection value_function)
    : storage_(std::move(storage)),
      input_(key_function.first, value_function.first),
      output_(key_function.second, value_function.second)
{
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename MapAdapter<Map, KeyBijection, ValueBijection>::iterator
MapAdapter<Map, KeyBijection, ValueBijection>::begin()
{
    return iterator(storage_->begin(), output_);
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename MapAdapter<Map, KeyBijection, ValueBijection>::iterator
MapAdapter<Map, KeyBijection, ValueBijection>::end()
{
    return iterator(storage_->end(), output_);
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename MapAdapter<Map, KeyBijection, ValueBijection>::size_type
MapAdapter<Map, KeyBijection, ValueBijection>::size() const
{
    return storage_->size();
}

template <typename Map, typename KeyBijection, typename ValueBijection>
std::pair<typename MapAdapter<Map, KeyBijection, ValueBijection>::iterator,
          bool>
MapAdapter<Map, KeyBijection, ValueBijection>::insert(const value_type& value)
{
    auto result = input_(value);
    auto insertion = storage_->insert(result);
    auto it = iterator(insertion.first, output_);
    return std::make_pair(iterator(it), insertion.second);
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename MapAdapter<Map, KeyBijection, ValueBijection>::const_iterator
MapAdapter<Map, KeyBijection, ValueBijection>::find(const key_type& key) const
{
    return iterator(storage_->find(input_.first(key)), output_);
}

template <typename Map, typename KeyBijection, typename ValueBijection>
typename MapAdapter<Map, KeyBijection, ValueBijection>::size_type
MapAdapter<Map, KeyBijection, ValueBijection>::erase(const key_type& key)
{
    return storage_->erase(input_.first(key));
}

} // namespace aux
