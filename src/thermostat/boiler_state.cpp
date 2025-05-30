#include "boiler_state.h"

bool BoilerState::operator==(BoilerState& other)
{
    return (*this).isCentralHeatingOn() == other.isCentralHeatingOn()
        && (*this).getCentralHeatingSetPoint() == other.getCentralHeatingSetPoint()
        && (*this).getCentralHeatingCurrentTemperature() == other.getCentralHeatingCurrentTemperature()
        && (*this).isHotWaterOn() == other.isHotWaterOn()
        && (*this).getHotWaterSetPoint() == other.getHotWaterSetPoint()
        && (*this).getHotWaterCurrentTemperature() == other.getHotWaterCurrentTemperature()
        && (*this).isHotWaterActive() == other.isHotWaterActive()
        && (*this).isFault() == other.isFault()
        && (*this).isFlameOn() == other.isFlameOn()
        && (*this).getModulation() == other.getModulation();
}