#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "energy_calculator.h"
#include "analytics.h"

int main()
{
    int requestedPulses;

    std::cout << "Smart Meter Pulse Simulator\n";
    std::cout << "Enter number of pulses: ";
    std::cin >> requestedPulses;

    if (requestedPulses <= 0)
    {
        std::cerr << "Error: Number of pulses must be greater than 0.\n";
        return 1;
    }

    // Read counter before generating new pulses
    std::ifstream before("/dev/smart_meter");

    if (!before)
    {
        std::cerr << "Error: Cannot read /dev/smart_meter\n";
        return 1;
    }

    int pulsesBefore = 0;
    before >> pulsesBefore;
    before.close();

    std::ofstream device("/dev/smart_meter");

    if (!device)
    {
        std::cerr << "Error: Cannot open /dev/smart_meter\n";
        return 1;
    }

    auto startTime = std::chrono::steady_clock::now();

    for (int i = 1; i <= requestedPulses; i++)
    {
        device << "1";
        device.flush();

        std::cout << "Pulse: " << i << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    auto endTime = std::chrono::steady_clock::now();

    device.close();

    // Read counter after generating pulses
    std::ifstream after("/dev/smart_meter");

    if (!after)
    {
        std::cerr << "Error: Cannot read /dev/smart_meter\n";
        return 1;
    }

    int totalPulses = 0;
    after >> totalPulses;
    after.close();

    int newPulses = totalPulses - pulsesBefore;

    double elapsedSeconds =
        std::chrono::duration<double>(endTime - startTime).count();

    double sessionEnergy =
        calculateEnergy(newPulses);

    double totalEnergy =
        calculateEnergy(totalPulses);

    double pulsesPerMinute =
        calculatePulsesPerMinute(newPulses, elapsedSeconds);

    double power =
        calculatePower(sessionEnergy, elapsedSeconds);

    std::cout << "\nSimulation completed.\n";
    std::cout << "New Pulses   : " << newPulses << std::endl;
    std::cout << "Total Pulses : " << totalPulses << std::endl;
    std::cout << "Session Energy : " << sessionEnergy << " kWh" << std::endl;
    std::cout << "Total Energy   : " << totalEnergy << " kWh" << std::endl;
    std::cout << "Elapsed Time   : " << elapsedSeconds << " seconds" << std::endl;
    std::cout << "Pulses/Min     : " << pulsesPerMinute << std::endl;
    std::cout << "Current Power  : " << power << " kW" << std::endl;

    return 0;
}
