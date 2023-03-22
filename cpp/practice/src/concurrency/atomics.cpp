
#include <ctime> // IWYU pragma: keep

#include "atomics.h"

PathGenerator::PathGenerator(uint32_t number_of_paths)
    : m_number_of_paths(number_of_paths)
{ }

void PathGenerator::my_secret_algorithm()
{
    /* Top secret */
}

void PathGenerator::generate_path()
{
#ifndef __clang__ // not yet supported in clang
    srand(time(0));

    static constexpr uint32_t my_favourite_number = 21;
    for(uint32_t i = 0; i < m_number_of_paths; ++i)
    {
        Path local_path{};
        for(uint32_t j = 0; j < my_favourite_number; ++j)
        {
            const PathPoint point{rand(), rand(), rand(), rand()};
            local_path.emplace_back(point);
        }
        m_is_used.wait(false);
        m_path = local_path;
        m_is_used = false;
        m_is_used.notify_one();
    }
    m_finished.test_and_set();
#endif
}

void PathGenerator::optimize_path()
{
#ifndef __clang__ // not yet supported in clang
    while(!m_finished.test())
    {
        m_is_used.wait(true);
        my_secret_algorithm();
        ++m_number_of_optimized_paths;
        m_is_used = true;
        m_is_used.notify_one();
    }
#endif
}

uint32_t PathGenerator::get_number_of_optimized_paths() const
{
    return m_number_of_optimized_paths;
}
