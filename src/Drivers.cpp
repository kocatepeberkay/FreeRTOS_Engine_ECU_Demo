#include "Drivers.hpp"
#include "HardwareSim.hpp"

#include <algorithm>
#include <atomic>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace {
std::atomic<TickType_t> lastWatchdogRefresh{0};
TickType_t watchdogTimeoutTicks = 0;
}

namespace AdcDriver {
void init() {}

float readTemperatureC() {
    const float normalized = static_cast<float>(HardwareSim::readAdcRaw()) / 4095.0f;
    return normalized * 150.0f; // Demo sensor range: 0..150 C.
}
}

namespace PwmDriver {
void init() { HardwareSim::writePwmPercent(0); }

void setFanSpeed(uint8_t percent) {
    HardwareSim::writePwmPercent(std::min<uint8_t>(percent, 100));
}
}

namespace CanDriver {
void init() {}
void transmit(const CanMessage& message) {
    HardwareSim::sendCanFrame(message.id, message.data, message.length);
}
}

namespace WatchdogDriver {
void init(uint32_t timeoutMs) {
    watchdogTimeoutTicks = pdMS_TO_TICKS(timeoutMs);
    refresh();
}

void refresh() { lastWatchdogRefresh.store(xTaskGetTickCount()); }

bool hasTimedOut() {
    return (xTaskGetTickCount() - lastWatchdogRefresh.load()) >= watchdogTimeoutTicks;
}

void reset() { refresh(); }
}
