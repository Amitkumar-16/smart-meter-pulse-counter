#include "energy_calculator.h"

double calculateEnergy(int pulseCount)
{
    const double meterConstant = 1000.0;

    return pulseCount / meterConstant;
}
