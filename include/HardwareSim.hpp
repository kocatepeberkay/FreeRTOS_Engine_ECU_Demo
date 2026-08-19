#pragma once

#include <cstdint>

namespace HardwareSim {
void init();
void waitForStart();
uint16_t readAdcRaw();
void writePwmPercent(uint8_t percent);
void sendCanFrame(uint32_t id, const uint8_t* data, uint8_t length);
bool faultKeyPressed();
void printStatus(float temperatureC, uint8_t fanPercent);
void printWatchdogReset();
}
