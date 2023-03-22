#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

namespace utils
{

template <auto K>
class Combination
{
public:
    explicit Combination(uint32_t n);

    std::array<uint8_t, K> next();
    bool is_valid() const;

private:
    std::string m_bitmask;
};

/************************* IMPL *************************/
template <auto K>
Combination<K>::Combination(uint32_t n)
    : m_bitmask(K, 1) // NOLINT
{
    m_bitmask.resize(n, 0);
}

template <auto K>
std::array<uint8_t, K> Combination<K>::next()
{
    std::array<uint8_t, K> indices{};
    for(size_t i = 0, j = 0; i < m_bitmask.size() and j < K; ++i)
    {
        if(m_bitmask[i])
        {
            indices[j++] = i;
        }
    }
    if(!std::prev_permutation(m_bitmask.begin(), m_bitmask.end()))
    {
        m_bitmask.clear();
    }

    return indices;
}

template <auto K>
bool Combination<K>::is_valid() const
{
    return !m_bitmask.empty();
}

} // namespace utils
