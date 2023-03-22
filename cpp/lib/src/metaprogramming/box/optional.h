
#pragma once

#include <compare>
#include <exception>
#include <utility>

namespace box
{
struct nullopt_t
{ };
struct in_place_t
{ };
struct bad_optional_access : public std::exception
{ };

constexpr nullopt_t nullopt{};
constexpr in_place_t in_place{};

template <typename T>
class optional
{
public:
    constexpr optional() noexcept;
    constexpr optional(box::nullopt_t) noexcept; // NOLINT(google-explicit-constructor)

    constexpr optional(const optional& other);

    // TODO: make it conditionally explicit
    template <typename U>
    explicit optional(const optional<U>& other);

    // TODO: implement is_nothrow_move_constructible_v type trait
    constexpr optional(optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>);

    template <typename U, typename... Args>
    constexpr explicit optional(box::in_place_t, std::initializer_list<U> ilist, Args&&... args);

    template <typename... Args>
    constexpr explicit optional(const box::in_place_t, Args&&... args);
    constexpr explicit optional(const T& data);

    constexpr T* operator->() noexcept;
    constexpr const T* operator->() const noexcept;

    constexpr T& operator*() & noexcept;
    constexpr const T& operator*() const& noexcept;

    constexpr T&& operator*() && noexcept;
    constexpr const T&& operator*() const&& noexcept;

    constexpr T& value() &;
    constexpr const T& value() const&;

    constexpr T&& value() &&;
    constexpr const T&& value() const&&;

    template <typename U>
    constexpr T value_or(U&& default_value) const&;
    template <typename U>
    constexpr T value_or(U&& default_value) &&;

    constexpr explicit operator bool() const noexcept;
    constexpr bool has_value() const noexcept;

    // C++20 NOTE: Since c++20 three-way comparison
    std::strong_ordering operator<=>(const box::optional<T>& opt) const noexcept = default;

private:
    // NOTE: if you instantiate a class template for two different argument types,
    // you get two different class types, so you are restricted to using the public interface,
    // unless you will make a friend with the template. Thanks to that:
    // template <typename U>
    // explicit optional(const optional<U>& other);
    // has access to the private members.
    template <typename>
    friend class optional;

    T m_data{};
    bool m_has_value{};
};

template <typename T>
constexpr box::optional<std::decay_t<T>> make_optional(T&& value);

template <typename T, typename... Args>
constexpr box::optional<T> make_optional(Args&&... args);

template <typename T, typename U, typename... Args>
constexpr box::optional<T> make_optional(std::initializer_list<U> il, Args&&... args);

} // namespace box

/************************ IMPL ************************/
#include "optional_impl.h"
