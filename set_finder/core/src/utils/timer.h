
#pragma once

#include <atomic>
#include <chrono>
#include <thread>

class Timer
{
public:
    enum class Type
    {
        SingleShot,
        Interval
    };

    explicit Timer(Type type = Type::SingleShot);
    ~Timer();

    template <typename Callback>
    void start(const Callback& callback, std::chrono::milliseconds msec);
    void restart();
    void stop();

private:
    Type m_type;
    std::atomic_bool m_restart;
    std::atomic_bool m_stop;
};

Timer::Timer(Timer::Type type)
    : m_type{type}
    , m_restart{false}
    , m_stop{false}
{ }

Timer::~Timer()
{
    m_stop = true;
}

template <typename Callback>
void Timer::start(const Callback& callback, std::chrono::milliseconds msec)
{
    m_stop = false;
    m_restart = false;

    const auto worker = [&, msec]() {
        do
        {
            auto start = std::chrono::steady_clock::now();
            do
            {
                std::this_thread::yield();

                if(m_restart)
                {
                    m_restart = false;
                    start = std::chrono::steady_clock::now();
                }

            } while(!m_stop && (std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::steady_clock::now() - start) < msec));

            if(!m_stop)
            {
                callback();
            }
        } while(!m_stop && m_type == Type::Interval);
    };

    std::thread worker_thread(worker);
    worker_thread.detach();
}

void Timer::restart()
{
    m_restart = true;
}

void Timer::stop()
{
    m_stop = true;
}
