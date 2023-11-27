
#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <thread>

class ThreadPool
{
public:
    explicit ThreadPool(uint32_t number_of_threads);
    ~ThreadPool();

    template <typename Task, typename... Args>
    void push_task(Task&& task, Args&&... args);

    void stop();
    bool is_finished() const;

private:
    void worker();

    std::atomic_bool m_stop{false};
    std::list<std::packaged_task<void()>> m_pending_tasks{};

    mutable std::mutex m_mtx{};
    std::condition_variable m_cv{};

    uint32_t m_number_of_threads{};
    uint32_t m_task_counter{};
    uint32_t m_finished_threads{};
};

ThreadPool::ThreadPool(uint32_t number_of_threads)
    : m_number_of_threads(number_of_threads)
{
    for(uint32_t i = 0; i < m_number_of_threads; ++i)
    {
        auto thread = std::thread(&ThreadPool::worker, this);
        thread.detach();
    }
}

ThreadPool::~ThreadPool()
{
    stop();
}

template <typename Task, typename... Args>
void ThreadPool::push_task(Task&& task, Args&&... args)
{
    std::scoped_lock lock{m_mtx};

    auto packaged_task =
        std::packaged_task<void()>(std::forward<Task>(task), std::forward<Args>(args)...);
    m_pending_tasks.emplace_back(std::move(packaged_task));
    ++m_task_counter;
    m_cv.notify_one();
}

void ThreadPool::stop()
{
    std::scoped_lock lock{m_mtx};
    m_stop = true;
    m_cv.notify_all();
}

bool ThreadPool::is_finished() const
{
    std::scoped_lock lock{m_mtx};
    return (m_stop) ? m_finished_threads == m_number_of_threads : m_task_counter == 0;
}

void ThreadPool::worker()
{
    std::unique_lock lock{m_mtx};
    while(!m_stop)
    {
        std::this_thread::yield();
        m_cv.wait(lock, [&] { return !m_pending_tasks.empty() || m_stop; });

        if(!m_pending_tasks.empty())
        {
            auto task = std::move(m_pending_tasks.front());
            m_pending_tasks.pop_front();

            lock.unlock();
            auto result = task.get_future();

            task();

            if(result.valid())
            {
                result.get();
            }
            lock.lock();
            --m_task_counter;
        }
    }
    ++m_finished_threads;
}
