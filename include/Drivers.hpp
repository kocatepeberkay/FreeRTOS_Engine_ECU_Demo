#pragma once

#include <cstdint>

struct CanMessage {
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
};

namespace AdcDriver {
void init();
float readTemperatureC();
}

namespace PwmDriver {
void init();
void setFanSpeed(uint8_t percent);
}

namespace CanDriver {
void init();
void transmit(const CanMessage& message);
}

namespace WatchdogDriver {
void init(uint32_t timeoutMs);
void refresh();
bool hasTimedOut();
void reset();
}
