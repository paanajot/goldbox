
#pragma once

/*******************************************************/

template <typename... Args>
auto sum(Args... args)
{
    return (... + args);
}

/*******************************************************/

template <typename T>
auto sum2(T arg)
{
    return arg;
}

template <typename T, typename... Args>
auto sum2(T arg, Args... args)
{
    return arg + sum2(args...);
}

/*******************************************************/

template <typename T, typename... Args>
auto sum3(T arg, Args... args)
{
    if constexpr((sizeof...(args) == 0))
    {
        return arg;
    }
    else
    {
        return arg + sum3(args...);
    }
}
