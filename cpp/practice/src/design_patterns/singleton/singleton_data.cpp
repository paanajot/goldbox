#include "singleton_data.h"

SingletonData::SingletonData()
{
    // NOLINTBEGIN
    m_city_population["Warszawa"] = 1'793'579;
    m_city_population["Krakow"] = 780'981;
    m_city_population["Lodz"] = 677'286;
    m_city_population["Wroclaw"] = 643'782;
    m_city_population["Poznan"] = 533'830;
    // NOLINTEND
}

SingletonData& SingletonData::get_instance()
{
    static SingletonData instance;
    return instance;
}

uint32_t SingletonData::get_population(const std::string& city) const
{
    return m_city_population.at(city);
}

void SingletonData::add_city(const std::string& city, uint32_t population)
{
    m_city_population[city] = population;
}
