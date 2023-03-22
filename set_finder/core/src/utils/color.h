
#pragma once

#include <string>

class Color
{
public:
    static void disable()
    {
        m_enabled = false;
    }

    static void enable()
    {
        m_enabled = true;
    }

    static std::string red()
    {
        return (m_enabled) ? "\033[31m" : "";
    }

    static std::string green()
    {
        return (m_enabled) ? "\033[32m" : "";
    }

    static std::string yellow()
    {
        return (m_enabled) ? "\033[33m" : "";
    }

    static std::string blue()
    {
        return (m_enabled) ? "\033[34m" : "";
    }

    static std::string reset()
    {
        return (m_enabled) ? "\033[0m" : "";
    }

private:
    static inline bool m_enabled = false;
};
