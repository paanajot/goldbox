
#pragma once

#include <utility>

namespace box
{
template <typename T>
constexpr optional<T>::optional() noexcept
    : m_has_value(false)
{ }

template <typename T>
constexpr optional<T>::optional(box::nullopt_t) noexcept
    : m_has_value(false)
{ }

template <typename T>
constexpr optional<T>::optional(const optional& other)
{
    if(other.m_has_value)
    {
        m_data = T(other.m_data);
    }
    m_has_value = other.m_has_value;
}

template <typename T>
template <typename U>
optional<T>::optional(const optional<U>& other)
{
    if(other.m_has_value)
    {
        m_data = T(other.m_data);
    }
    m_has_value = other.m_has_value;
}

template <typename T>
constexpr optional<T>::optional(optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
{
    if(other.m_has_value)
    {
        m_data = T(std::move(other.m_data));
    }
    m_has_value = other.m_has_value;
}

template <typename T>
template <typename... Args>
constexpr optional<T>::optional(const box::in_place_t, Args&&... args)
    : m_data(T(std::forward<Args>(args)...))
    , m_has_value(true)
{ }

template <typename T>
constexpr optional<T>::optional(const T& data)
    : m_data(data)
    , m_has_value(true)
{ }

template <typename T>
template <typename U, typename... Args>
constexpr optional<T>::optional(box::in_place_t, std::initializer_list<U> ilist, Args&&... args)
    : m_data(T(ilist, std::forward<Args>(args)...))
    , m_has_value(true)
{ }

template <typename T>
constexpr T* optional<T>::operator->() noexcept
{
    return &m_data;
}

template <typename T>
constexpr const T* optional<T>::operator->() const noexcept
{
    return &m_data;
}

template <typename T>
constexpr T& optional<T>::operator*() & noexcept
{
    return m_data;
}

template <typename T>
constexpr const T& optional<T>::operator*() const& noexcept
{
    return m_data;
}

template <typename T>
constexpr T&& optional<T>::operator*() && noexcept
{
    return std::move(m_data);
}

template <typename T>
constexpr const T&& optional<T>::operator*() const&& noexcept
{
    return std::move(m_data);
}

template <typename T>
constexpr T& optional<T>::value() &
{
    // This looks awful, but we avoid copy / paste
    return (const_cast<T&>((const_cast<const optional<T>&>(*this)).value()));
}

template <typename T>
constexpr const T& optional<T>::value() const&
{
    if(!m_has_value)
    {
        throw box::bad_optional_access();
    }
    return m_data;
}

template <typename T>
constexpr T&& optional<T>::value() &&
{
    return (const_cast<T&&>((const_cast<const optional<T>&&>(*this)).value()));
}

template <typename T>
constexpr const T&& optional<T>::value() const&&
{
    if(!m_has_value)
    {
        throw box::bad_optional_access();
    }

    // NOTE: generally using std::move in return is not a good practice, it prevents copy elision.
    // Nevertheless in this case copy elision would not occur and we want object to be moved.
    return std::move(m_data);
}

template <typename T>
template <typename U>
constexpr T optional<T>::value_or(U&& default_value) const&
{
    return bool(*this) ? **this : static_cast<T>(std::forward<U>(default_value));
}

template <typename T>
template <typename U>
constexpr T optional<T>::value_or(U&& default_value) &&
{
    return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(default_value));
}

template <typename T>
constexpr optional<T>::operator bool() const noexcept
{
    return m_has_value;
}

template <typename T>
constexpr bool optional<T>::has_value() const noexcept
{
    return m_has_value;
}

template <typename T>
constexpr box::optional<std::decay_t<T>> make_optional(T&& value)
{
    return box::optional<std::decay_t<T>>(std::forward<T>(value));
}

template <typename T, typename... Args>
constexpr box::optional<T> make_optional(Args&&... args)
{
    return box::optional<T>(box::in_place, std::forward<Args>(args)...);
}

template <typename T, typename U, typename... Args>
constexpr box::optional<T> make_optional(std::initializer_list<U> il, Args&&... args)
{
    return box::optional<T>(in_place, il, std::forward<Args>(args)...);
}
} // namespace box
