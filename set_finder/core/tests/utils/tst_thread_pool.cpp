#include <gtest/gtest.h>

#include <chrono>

#include "utils/thread_pool.h"

using namespace std::chrono_literals;

class MySupervisor
{
public:
    void increase_counter()
    {
        ++m_counter;
    }

    uint32_t get_counter() const
    {
        return m_counter;
    }

private:
    std::atomic<uint32_t> m_counter{};
};

struct TestData
{
    uint32_t number_of_threads{};
    uint32_t number_of_tasks{};
};

class ThreadPoolTest : public testing::TestWithParam<TestData>
{ };

::std::ostream& operator<<(::std::ostream& os, const TestData& test_data)
{
    return os << "threads: " << test_data.number_of_threads
              << " ,tasks: " << test_data.number_of_tasks;
}

TEST_P(ThreadPoolTest, thread_pool_test)
{
    const auto params = GetParam();
    const auto number_of_threads = params.number_of_threads;
    const auto number_of_tasks = params.number_of_tasks;

    ThreadPool thread_pool{number_of_threads};

    MySupervisor supervisor{};

    for(uint32_t i = 0; i < number_of_tasks; ++i)
    {
        thread_pool.push_task([&]() { supervisor.increase_counter(); });
    }

    while(!thread_pool.is_finished())
    {
        std::this_thread::yield();
    }

    EXPECT_EQ(supervisor.get_counter(), number_of_tasks);

    thread_pool.stop();
    while(!thread_pool.is_finished())
    {
        std::this_thread::yield();
    }
}

INSTANTIATE_TEST_SUITE_P(ThreaPool,
                         ThreadPoolTest,
                         testing::Values(TestData{1, 1},
                                         TestData{3, 1},
                                         TestData{1, 3},
                                         TestData{4, 7},
                                         TestData{7, 4},
                                         TestData{5, 5},
                                         TestData{4, 100}));
