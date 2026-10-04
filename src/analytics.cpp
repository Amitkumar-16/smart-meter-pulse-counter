#include "analytics.h"

double calculatePulsesPerMinute(int pulseCount, double elapsedSeconds)
{
    if (elapsedSeconds <= 0)
        return 0.0;

    return pulseCount / (elapsedSeconds / 60.0);
}

double calculatePower(double energyKWh, double elapsedSeconds)
{
    if (elapsedSeconds <= 0)
        return 0.0;

    return energyKWh / (elapsedSeconds / 3600.0);
}
