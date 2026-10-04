# Smart-Meter Pulse Counter & Analytics Agent

## 1. Project Overview

The Smart-Meter Pulse Counter & Analytics Agent is a Linux-based project that simulates the pulse output of an electricity meter and processes those pulses using a Linux character device driver.

The Linux kernel driver maintains the pulse count and provides a character device at `/dev/smart_meter`. A C++ user-space application communicates with this device, generates meter pulses, reads the pulse count, and performs basic energy and usage calculations.

The project was selected to demonstrate practical concepts from Linux, C/C++, operating systems, computer architecture, and hardware-software interaction.

## 2. Objective

The main objective of this project is to develop a simple smart-meter monitoring system using a Linux character device driver.

The project performs the following operations:

- Receives simulated electricity-meter pulses.
- Maintains the pulse count inside the Linux kernel driver.
- Provides access to the counter through `/dev/smart_meter`.
- Allows a C++ user-space application to communicate with the driver.
- Calculates energy consumption from the number of pulses.
- Calculates pulse rate in pulses per minute.
- Estimates power consumption from energy and elapsed time.
- Demonstrates communication between user space and kernel space.

## 3. System Architecture

The overall data flow of the project is:

```text
+----------------------+
| Smart Meter Pulses   |
|     (Simulated)      |
+----------+-----------+
           |
           v
+----------------------+
| Linux Character      |
| Device Driver        |
+----------+-----------+
           |
           v
+----------------------+
|   /dev/smart_meter   |
+----------+-----------+
           |
           v
+----------------------+
| C++ User-Space       |
| Application          |
+----------+-----------+
           |
           v
+----------------------+
| Analytics Module     |
+----------+-----------+
           |
           v
+----------------------+
| Energy / Power /     |
| Pulse Rate           |
+----------------------+

4. Technologies Used
- Linux Ubuntu
- C
- C++
- C++17
- Linux Kernel Module
- Linux Character Device Driver
- Kernel file_operations
- alloc_chrdev_region()
- cdev
- device_create()
- copy_to_user()
- copy_from_user()
- Mutex synchronization
- Make
- GCC / G++
- Git / GitHub
5. Project Structure
smart-meter-pulse-counter/
│
├── driver/
│   ├── Makefile
│   └── smart_meter_driver.c
│
├── include/
│   ├── analytics.h
│   └── energy_calculator.h
│
├── simulator/
│   └── pulse_generator.cpp
│
├── src/
│   ├── analytics.cpp
│   └── energy_calculator.cpp
│
├── docs/
│   └── architecture.md
│
├── Makefile
├── README.md
└── .gitignore

Directory Description
driver/
Contains the Linux kernel character device driver source code and its Makefile.
include/
Contains the header files for the C++ analytics and energy calculation modules.
src/
Contains the implementations of the C++ calculation and analytics functions.
simulator/
Contains the user-space C++ program that generates simulated meter pulses and communicates with the device driver.
docs/
Contains additional project documentation and architecture information.
6. How to Build
First, build the C++ user-space application:
make

Build the Linux kernel driver:
make driver

After successful compilation, the kernel module will be generated inside the driver directory.
The generated build files are excluded from Git using .gitignore.
7. How to Run
Step 1: Load the kernel driver
sudo insmod driver/smart_meter_driver.ko

Step 2: Verify the device
ls -l /dev/smart_meter

Step 3: Run the C++ simulator
sudo ./pulse_generator

The program asks for the number of pulses to generate.
For example:
Smart Meter Pulse Simulator
Enter number of pulses: 5

The program generates the requested pulses and displays the calculated results.
Example:
Simulation completed.
New Pulses   : 5
Total Pulses : 5
Session Energy : 0.005 kWh
Total Energy   : 0.005 kWh
Elapsed Time   : 5 seconds
Pulses/Min     : approximately 60
Current Power  : approximately 3.6 kW

Step 4: Unload the driver
After testing:
sudo rmmod smart_meter_driver

8. Technical Concepts Demonstrated
Linux Kernel Module
The project contains a loadable Linux kernel module that can be inserted and removed using insmod and rmmod.
Character Device Driver
The driver creates a character device named:
/dev/smart_meter

The device provides an interface between the user-space application and the kernel.
User Space and Kernel Space
The C++ application runs in user space, while the pulse counter is maintained by the kernel driver.
Data is transferred using Linux kernel functions such as:
copy_to_user()
copy_from_user()

File Operations
The driver implements operations such as:
open()
read()
write()
release()

through the Linux file_operations structure.
Synchronization
A mutex is used to protect access to the shared pulse counter:
pulse_lock

This helps prevent concurrent access from causing inconsistent counter updates.
Hardware-Software Interaction
Although the physical electricity meter is simulated in this project, the architecture represents how a hardware pulse source could communicate with software through a device driver.
C++ Analytics
The C++ application calculates:
- Energy consumption
- Total energy
- Pulse rate
- Estimated power
9. Testing
The project was tested using both valid and invalid pulse inputs.
Valid Pulse Test
A pulse input of:
1

is accepted by the driver and increments the pulse counter.
Invalid Input Test
Invalid input such as:
999

is rejected by the driver.
The driver returns an error instead of incrementing the counter.
C++ Integration Test
The C++ simulator was tested by generating multiple pulses and comparing the pulse count before and after the simulation.
The application successfully calculated the new pulses, total pulses, energy, pulse rate, and estimated power.
10. Limitations
The current implementation has some limitations:
- The physical electricity meter is not directly connected to the system.
- Meter pulses are simulated by the C++ application.
- The pulse counter is stored in kernel memory.
- The counter is reset when the driver is unloaded and loaded again.
- The device currently requires elevated privileges for access.
- The current project does not use persistent storage for historical meter readings.
- The energy calculation uses a fixed meter constant.
11. Future Improvements
Possible future improvements include:
- Connect the driver to an actual pulse-generating hardware sensor.
- Add persistent storage for meter readings.
- Add timestamped energy records.
- Provide daily and monthly consumption statistics.
- Improve device permissions so normal users can access the device.
- Add a configuration interface for the meter constant.
- Add more robust input validation.
- Develop a monitoring interface for real-time energy information.
- Add support for multiple meter channels.
12. Conclusion
The project demonstrates how a Linux character device driver can provide an interface between a hardware-like pulse source and a C++ user-space application.
It combines Linux kernel programming, character devices, synchronization, user-kernel data transfer, C++ programming, and basic energy analytics into a single system.
