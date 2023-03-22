
#pragma once

#include "electricity.h"
#include "fuel_injector.h"
#include "ignition.h"

// NOTE: Provides a simplified interface.
class EngineFacade
{
public:
    void start_engine()
    {
        if(m_electricity.check_electric_voltage())
        {
            m_electricity.on();
            m_fuel_injector.inject();
            m_fuel_injector.start();
            m_ignition.on();
        }
    }

    void stop_engine()
    {
        m_fuel_injector.stop();
        m_electricity.off();
    }

private:
    FuelInjector m_fuel_injector{};
    Ignition m_ignition{};
    Electricity m_electricity{};
};
