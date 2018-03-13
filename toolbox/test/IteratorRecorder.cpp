#include "gtest/gtest.h"
#include <toolbox/Iterator.h>
#include <toolbox/IteratorRecorder.h>

TEST(Toolbox, IteratorRecorder)
{
    using Input = std::vector<int>;
    using Recorder = toolbox::IteratorRecorder<Input::const_iterator>;
    auto input = Input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto recorder = Recorder{input.cbegin()};
    EXPECT_EQ(1, *recorder);
    EXPECT_EQ(1, *recorder);
    EXPECT_EQ(2, *(++recorder));
    EXPECT_EQ(1, *(--recorder));
    EXPECT_EQ(1, *recorder);
    auto begin = Recorder(input.cbegin());
    auto end = Recorder(input.cend());
    auto back = toolbox::back(begin, end);
    auto reverse = Input{};
    std::copy(std::make_reverse_iterator(back),
              std::make_reverse_iterator(begin), std::back_inserter(reverse));
    EXPECT_EQ((Input{9, 8, 7, 6, 5, 4, 3, 2, 1}), reverse);
}
