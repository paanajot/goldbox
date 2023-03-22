
#include "shared_mutex.h"

#include <mutex>

PhoneBook::Number PhoneBook::read_number(const Name& name) const
{
    std::shared_lock lock(m_shared_mutex);
    if(m_phone_book.find(name) != m_phone_book.end())
    {
        return m_phone_book.at(name);
    }
    return {};
}

void PhoneBook::add_number(const Name& name, Number number)
{
    std::unique_lock lock(m_shared_mutex);
    m_phone_book[name] = number;
}
