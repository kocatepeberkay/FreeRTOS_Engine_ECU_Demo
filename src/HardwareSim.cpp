#include "HardwareSim.hpp"

#include <algorithm>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>

namespace {
std::mutex consoleMutex;
std::mt19937 randomEngine{std::random_device{}()};
std::uniform_int_distribution<int> noise{-18, 18};
int adcValue = 2250;
}

namespace HardwareSim {
void init() {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "\n==================================================\n"
              << "  VIRTUAL ENGINE ECU - FreeRTOS Windows Demo\n"
              << "==================================================\n\n"
              << "  Simulated engine temperature is read, the fan is\n"
              << "  controlled and CAN telemetry is transmitted.\n\n"
              << "  KEYS\n\n"
              << "  S       : Start simulation\n"
              << "  F       : Trigger watchdog fault (while running)\n"
              << "  Ctrl+C  : Exit\n\n";
}

void waitForStart() {
    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "  Press S to start... ";
    }

    for (;;) {
        const int key = _getch();
        if (key == 's' || key == 'S') {
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cout << "\n\n  Simulation started.\n";
            return;
        }
    }
}

uint16_t readAdcRaw() {
    adcValue += noise(randomEngine);
    adcValue = std::clamp(adcValue, 1600, 3400);
    return static_cast<uint16_t>(adcValue);
}

void writePwmPercent(uint8_t percent) {
    (void)percent; // A real target would write a timer compare register here.
}

void sendCanFrame(uint32_t id, const uint8_t* data, uint8_t length) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "[CAN TASK] Telemetry transmitted\n\n"
              << "  CAN ID     : 0x" << std::hex << std::uppercase << id << "\n"
              << "  Data length: " << std::dec << static_cast<int>(length) << " bytes\n"
              << "  Payload    : ";
    for (uint8_t i = 0; i < length; ++i) {
        std::cout << std::hex << std::uppercase;
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << ' ';
    }
    std::cout << std::dec << std::nouppercase << std::setfill(' ') << "\n";
}

bool faultKeyPressed() {
    if (_kbhit() == 0) return false;
    const int key = _getch();
    return key == 'f' || key == 'F';
}

void printStatus(float temperatureC, uint8_t fanPercent) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "\n--------------------------------------------------\n\n"
              << "[CONTROL TASK] New sensor sample\n\n"
              << std::fixed << std::setprecision(1)
              << "  Engine temperature : " << temperatureC << " C\n"
              << "  Fan command        : " << static_cast<int>(fanPercent) << " %\n\n";
}

void printWatchdogReset() {
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
              << "  WATCHDOG TIMEOUT\n\n"
              << "  Simulated ECU reset completed.\n"
              << "  Fan was placed in the 100% safe state.\n"
              << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
}
}
