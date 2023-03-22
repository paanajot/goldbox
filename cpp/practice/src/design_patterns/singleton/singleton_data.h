
#pragma once

#include <cstdint>
#include <map>
#include <string>

// https://www.udemy.com/course/patterns-cplusplus/
// A design pattern everyone loves to hate... but it is really that bad?
class SingletonData
{
public:
    static SingletonData& get_instance();

    uint32_t get_population(const std::string& city) const;
    void add_city(const std::string& city, uint32_t population);

    SingletonData(const SingletonData&) = delete;
    void operator=(const SingletonData&) = delete;

private:
    SingletonData();

    std::map<std::string, uint32_t> m_city_population;
};
