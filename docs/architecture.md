# System Architecture

## Overview

The Smart-Meter Pulse Counter & Analytics Agent uses a Linux character device driver to provide communication between a simulated meter pulse source and a C++ user-space application.

## Architecture Flow

```text
+----------------------+
| Smart Meter Pulses   |
|     (Simulated)      |
+----------+-----------+
           |
           v
+----------------------+
| Linux Kernel         |
| Character Driver     |
+----------+-----------+
           |
           v
+----------------------+
| /dev/smart_meter     |
+----------+-----------+
           |
           v
+----------------------+
| C++ User-Space       |
| Pulse Simulator      |
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

Component Description
1. Pulse Source
The project uses the C++ simulator to generate pulses representing the output of an electricity meter.
2. Linux Character Device Driver
The kernel module maintains the pulse counter and exposes it through the character device:
/dev/smart_meter
The driver implements open, read, write, and release operations.
3. User-Kernel Communication
The user-space application communicates with the driver through the device file.
The driver uses:
- copy_to_user() to transfer data from kernel space to user space.
- copy_from_user() to receive data from user space.
4. Synchronization
A mutex protects the shared pulse counter so that access to the counter is synchronized.
5. C++ Analytics
After obtaining the pulse count, the C++ application calculates:
- New pulses
- Total pulses
- Energy consumption
- Pulses per minute
- Estimated power
Data Flow
Pulse Generated
      |
      v
Driver write()
      |
      v
Pulse Counter
      |
      v
Driver read()
      |
      v
C++ Application
      |
      v
Analytics Functions
      |
      v
Energy / Power Results

Software Layers
+--------------------------------+
| C++ User-Space Application     |
| Pulse Simulator + Analytics    |
+--------------------------------+
               |
               | /dev/smart_meter
               v
+--------------------------------+
| Linux Kernel Space             |
| Smart Meter Character Driver  |
+--------------------------------+
               |
               v
+--------------------------------+
| Linux Kernel / Device Layer    |
+--------------------------------+

Main Linux Concepts
The project demonstrates:
- Loadable kernel modules
- Character devices
- Device numbers
- cdev
- File operations
- User-space/kernel-space communication
- Mutex synchronization
- Device creation
- Kernel logging
