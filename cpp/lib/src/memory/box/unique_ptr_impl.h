
#pragma once

#include <utility>

namespace box
{
template <typename T>
unique_ptr<T>::unique_ptr(T* data)
    : m_data(data)
{ }

template <typename T>
unique_ptr<T>::~unique_ptr()
{
    if(m_data)
    {
        delete m_data;
    }
}

template <typename T>
unique_ptr<T>::unique_ptr(unique_ptr&& ptr)
    : m_data(std::move(ptr.m_data))
{
    ptr.m_data = nullptr;
}

template <typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr&& ptr)
{
    if(m_data)
    {
        delete m_data;
    }
    m_data = std::move(ptr.m_data);
    ptr.m_data = nullptr;
    return *this;
}

template <typename T>
unique_ptr<T>::operator bool() const
{
    return m_data != nullptr;
}

template <typename T>
T& unique_ptr<T>::operator*() const
{
    return *m_data;
}

template <typename T>
T* unique_ptr<T>::operator->() const
{
    return this->get();
}

template <typename T>
T* unique_ptr<T>::release()
{
    T* tmp_data = m_data;
    m_data = nullptr;
    return tmp_data;
}

template <typename T>
T* unique_ptr<T>::get() const
{
    return m_data;
}

template <typename T>
void unique_ptr<T>::reset(T* ptr)
{
    auto* old_data = m_data;
    m_data = ptr;
    if(old_data)
    {
        delete old_data;
    }
}

template <typename T>
void unique_ptr<T>::swap(unique_ptr<T>& ptr)
{
    auto* tmp_data = m_data;
    m_data = ptr.m_data;
    ptr.m_data = tmp_data;
}

template <typename T, typename... Args>
auto make_unique(Args&&... args)
{
    return box::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace box
