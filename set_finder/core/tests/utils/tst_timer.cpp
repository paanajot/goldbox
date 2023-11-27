#include <gtest/gtest.h>

#include "utils/timer.h"

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

TEST(TimerTest, single_shot)
{
    MySupervisor supervisor;
    Timer timer{Timer::Type::SingleShot};

    const auto callback = [&supervisor]() { supervisor.increase_counter(); };

    const auto msec{5ms};
    timer.start(callback, msec);
    std::this_thread::sleep_for(msec / 2);
    EXPECT_EQ(supervisor.get_counter(), 0);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 1);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 1);
}

TEST(TimerTest, interval)
{
    MySupervisor supervisor;
    Timer timer{Timer::Type::Interval};

    const auto callback = [&supervisor]() { supervisor.increase_counter(); };

    const auto msec{5ms};
    timer.start(callback, msec);
    std::this_thread::sleep_for(msec / 2);
    EXPECT_EQ(supervisor.get_counter(), 0);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 1);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 2);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 3);
}

TEST(TimerTest, interval_stop_start)
{
    MySupervisor supervisor;
    Timer timer{Timer::Type::Interval};

    const auto callback = [&supervisor]() { supervisor.increase_counter(); };

    const auto msec{5ms};
    timer.start(callback, msec);
    std::this_thread::sleep_for(msec / 2);
    EXPECT_EQ(supervisor.get_counter(), 0);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 1);

    timer.stop();

    std::this_thread::sleep_for(msec + 1ms);
    ASSERT_EQ(supervisor.get_counter(), 1);

    timer.start(callback, msec);
    std::this_thread::sleep_for(msec + 1ms);
    ASSERT_EQ(supervisor.get_counter(), 2);
}

TEST(TimerTest, interval_restart)
{
    MySupervisor supervisor;
    Timer timer{Timer::Type::Interval};

    const auto callback = [&supervisor]() { supervisor.increase_counter(); };

    const auto msec{5ms};
    timer.start(callback, msec);
    std::this_thread::sleep_for(msec / 2);
    EXPECT_EQ(supervisor.get_counter(), 0);

    std::this_thread::sleep_for(msec);
    ASSERT_EQ(supervisor.get_counter(), 1);

    std::this_thread::sleep_for(msec / 2);
    timer.restart();

    std::this_thread::sleep_for(2 * msec / 3);
    ASSERT_EQ(supervisor.get_counter(), 1);

    std::this_thread::sleep_for(msec / 2);
    ASSERT_EQ(supervisor.get_counter(), 2);
}
