
#pragma once

#include <utility>

namespace box
{
template <typename T>
class unique_ptr
{
public:
    explicit unique_ptr(T* data);
    ~unique_ptr();

    unique_ptr(unique_ptr&& ptr);
    unique_ptr<T>& operator=(unique_ptr&& ptr);
    explicit operator bool() const;

    T& operator*() const;
    T* operator->() const;
    T* release();
    T* get() const;
    void reset(T* ptr);
    void swap(unique_ptr<T>& ptr);

    unique_ptr(const unique_ptr& ptr) = delete;
    unique_ptr<T>& operator=(const unique_ptr& ptr) = delete;

private:
    T* m_data;
};

template <typename T, typename... Args>
auto make_unique(Args&&... args);

} // namespace box

/************************ IMPL ************************/
#include "unique_ptr_impl.h"
