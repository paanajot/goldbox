
#pragma once

#include <cstdint>
#include <string>

// NOTE: use lambdas for complex initialization
void lambda_init(uint32_t accel)
{
    // NOLINTBEGIN [readability-magic-numbers]
    // Bad example
    [[maybe_unused]] uint32_t velocity_1{0};
    if(accel > 10)
    {
        velocity_1 = 100;
    }
    else if(accel > 5)
    {
        velocity_1 = 50;
    }

    // Good example - we can mark variable as a const
    [[maybe_unused]] const uint32_t velocity_2 = [accel]() {
        if(accel > 10)
        {
            return 100;
        }
        else if(accel > 5)
        {
            return 50;
        }

        return 0;
    }(); // sometimes it is hard to see that you invoke lambda inline,
    // so if you use c++17 you can use std::invoke for that
    // NOLINTEND
}

// NOTE: pass-be-value with move or pass-by-reference?
class Howl
{
public:
    /* pass-by-reference
       when we pass lvalue or rvalue it is first binded to the reference then copied to m_name

    explicit Howl(const std::string& name)
        : m_name{name}
    { }
    */

    /* pass-by-value
       passed lvalue is copied to name, then it's moved to m_name
       passed rvalue is moved to name, then it's moved to m_name

    explicit Howl(std::string name)
        : m_name{std::move(name)}
    { }
    */

    // pass-by-reference
    // passed lvalue is binded to name, then it's copier to m_name
    explicit Howl(const std::string& name)
        : m_name{name}
    { }

    // pass-by-rvalue
    // passed lvalue is binded to name, then it's moved to m_name
    explicit Howl(std::string&& name)
        : m_name{std::move(name)}
    { }

    // So which one is better? If you pass a lot of rvalues, then pass-by-value with std::move is better,
    // then passing by const reference. The last option is the most optimal, but it requires code repetition.

    // This setter is bad!!!
    // When doing a move assignment the destination string's existing storage must be deallocated
    // as it takes over the storage from the source string. When we copy a string to another string
    // and memory allocated will be enough to store new string, it will be reused. In that case
    // no additional/unnecessary deallocation occurs.
    void set_name(std::string name) noexcept
    {
        m_name = std::move(name);
    }

private:
    std::string m_name;
};
