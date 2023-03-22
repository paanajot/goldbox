#include <gtest/gtest.h>
#include <thread>

#include "atomics.h"

TEST(concurrency, atomics)
{
    const auto number_of_paths = 12;
    PathGenerator generator{number_of_paths};

    std::thread t1(&PathGenerator::generate_path, &generator);
    std::thread t2(&PathGenerator::optimize_path, &generator);

    t1.join();
    t2.join();

    EXPECT_EQ(number_of_paths, generator.get_number_of_optimized_paths());
}
