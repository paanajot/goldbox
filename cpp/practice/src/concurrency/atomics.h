
#pragma once

#include <atomic>
#include <cstdint>
#include <vector>

// C++20 NOTE: c++20 adds wait(), notify_one() and notify_all() methods to the atomic type.
// It provides same functionalities as condition_variable, but looks to be much simpler to use.
// You don't have to use mutex, predicate and unique_lock anymore like in the case of cv if you want to be safe.

// NOTE: Path generator is just a class that shows how the atomic works.
// It's very poorly written, you can run optimize_path only in a one thread instance.
class PathGenerator
{
public:
    explicit PathGenerator(uint32_t number_of_paths);

    struct PathPoint
    {
        int x{};
        int y{};
        int timestamp{};
        int theta{};
    };

    using Path = std::vector<PathPoint>;

    void generate_path();
    void optimize_path();
    uint32_t get_number_of_optimized_paths() const;

private:
    std::atomic_bool m_is_used{true};
    std::atomic_uint32_t m_number_of_optimized_paths{0};

    // NOTE: Unlike all specializations of std::atomic, it is guaranteed to be lock-free.
    std::atomic_flag m_finished{};

    Path m_path{};
    uint32_t m_number_of_paths{}; // NOLINT

    void my_secret_algorithm();
};
