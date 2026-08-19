# FreeRTOS Engine ECU Demo

A modern C++ embedded software demo that simulates a small engine control unit on Windows using the real FreeRTOS scheduler and queue APIs.

The project demonstrates RTOS task design, low-level driver abstraction, periodic sensor acquisition, PWM fan control, CAN telemetry packing, inter-task communication, and watchdog-based fault recovery without requiring physical hardware.

> This is a Windows simulation intended to demonstrate embedded software architecture. The hardware layer is simulated and does not access MCU registers or provide hard real-time guarantees.

## Technical Highlights

- Runs two FreeRTOS tasks with explicit priorities and blocking behavior
- Uses `vTaskDelayUntil` for fixed-period 5-second sensor sampling
- Transfers the latest telemetry through a single-element FreeRTOS queue
- Converts simulated 12-bit ADC data into an engine temperature value
- Calculates and applies a bounded PWM fan command
- Packs temperature and fan data into a CAN-style frame with ID `0x180`
- Models a 10-second watchdog timeout and a 100% fan safe state
- Separates application logic, driver interfaces, and simulated hardware access
- Pins FreeRTOS Kernel `V11.2.0` as a Git submodule for reproducible builds

## Architecture

```text
                         +----------------+
                         |  ControlTask   |
                         +-------+--------+
                                 |
             +-------------------+-------------------+
             |                   |                   |
             v                   v                   v
        ADC Driver          PWM Driver        Watchdog Driver
             |                   |                   |
             +-------------------+-------------------+
                                 |
                          HardwareSim Layer

        ControlTask -- FreeRTOS Queue --> CanTask --> CAN Driver
```

`ControlTask` samples the simulated ADC, calculates the fan duty cycle, creates the telemetry payload, and refreshes the watchdog. `CanTask` blocks on the queue and transmits each available frame through the CAN driver interface.

The driver layer exposes hardware-oriented ADC, PWM, CAN, and watchdog APIs. Their current implementations delegate to `HardwareSim`, allowing the task and application logic to remain independent from platform-specific I/O.

## CAN Telemetry

| Field | Value |
|---|---|
| CAN ID | `0x180` |
| Payload length | 3 bytes |
| Bytes 0–1 | Engine temperature in tenths of a degree Celsius, big-endian |
| Byte 2 | Fan command in percent |

Example payload:

```text
CAN ID: 0x180
Payload: 03 38 18
```

## Build

Requirements:

- Windows 10 or 11
- Visual Studio 2022 or newer with **Desktop development with C++**
- CMake 3.21 or newer
- Git

Clone the repository with its FreeRTOS submodule:

```powershell
git clone --recurse-submodules https://github.com/kocatepeberkay/FreeRTOS_Engine_ECU_Demo.git
cd FreeRTOS_Engine_ECU_Demo
```

Configure and build:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

## Run

```powershell
.\build\Release\VirtualEngineECU.exe
```

- Press `S` to start the simulation.
- A new ADC, PWM, and CAN telemetry sample is produced every 5 seconds.
- Press `F` while running to stop watchdog refreshes and observe the simulated timeout and safe-state response.
- Press `Ctrl+C` to exit.

Sample console output is available in [`samples/expected-output.txt`](samples/expected-output.txt).

## Project Structure

```text
include/                     Public interfaces and FreeRTOS configuration
src/                         Tasks, drivers, hardware simulation, and startup
samples/expected-output.txt  Example runtime output
third_party/FreeRTOS-Kernel  FreeRTOS V11.2.0 submodule
CMakeLists.txt               MSVC/CMake build configuration
```

## Roadmap

- Unit tests for fan control and CAN payload encoding
- Configurable sampling and watchdog periods
- Additional diagnostic CAN frames
- Hardware-backed driver implementation for an MCU target
- Continuous integration with GitHub Actions

## License

The FreeRTOS Kernel submodule is distributed under its own MIT license. No separate license has been assigned to the application code yet.
