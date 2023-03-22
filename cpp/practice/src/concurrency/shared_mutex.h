
#pragma once

#include <shared_mutex>
#include <stdint.h>
#include <string>
#include <unordered_map>

// C++17 NOTE: shared_mutex solves readers–writers problem
class PhoneBook
{
public:
    using Number = uint32_t;
    using Name = std::string;

    Number read_number(const Name& name) const;
    void add_number(const Name& name, Number number);

private:
    std::unordered_map<Name, Number> m_phone_book{};
    mutable std::shared_mutex m_shared_mutex{};
};
