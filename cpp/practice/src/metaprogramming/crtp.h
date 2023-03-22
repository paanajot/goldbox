
#pragma once

#include <string>

// NOTE: CRTP Curiously Recurring Template Pattern
// One of the usage of CRTP pattern is to create static interfaces. According to the performance test described here:
// https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c
// it can be much more faster in the runtime.
template <typename CRTP>
class Color
{
public:
    std::string get_color() const
    {
        const auto& instance = static_cast<const CRTP&>(*this);
        return instance.get_color();
    }

private:
    // That's the solution to prevent use of the wrong class, such as:
    // class GreenColor : public Color<RedColor>
    Color() = default;
    friend CRTP;
};

class GreenColor : public Color<GreenColor>
{
public:
    std::string get_color() const
    {
        return "green";
    }
};

class RedColor : public Color<RedColor>
{
public:
    std::string get_color() const
    {
        return "red";
    }
};
