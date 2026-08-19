#include "Tasks.hpp"
#include "Drivers.hpp"
#include "HardwareSim.hpp"

#include <algorithm>
#include <cstdint>

extern "C" {
#include "task.h"
}

namespace {
QueueHandle_t canQueue = nullptr;
constexpr TickType_t controlPeriod = pdMS_TO_TICKS(5000);

uint8_t calculateFanSpeed(float temperatureC) {
    if (temperatureC < 75.0f) return 0;
    if (temperatureC >= 105.0f) return 100;
    return static_cast<uint8_t>((temperatureC - 75.0f) * (100.0f / 30.0f));
}

void ControlTask(void*) {
    TickType_t lastWake = xTaskGetTickCount();
    bool faultActive = false;

    for (;;) {
        if (HardwareSim::faultKeyPressed() && !faultActive) {
            faultActive = true;
        }

        const float temperature = AdcDriver::readTemperatureC();
        const uint8_t fan = calculateFanSpeed(temperature);
        PwmDriver::setFanSpeed(fan);
        HardwareSim::printStatus(temperature, fan);

        const uint16_t temperatureTenths = static_cast<uint16_t>(temperature * 10.0f);
        CanMessage message{0x180, 3, {
            static_cast<uint8_t>(temperatureTenths >> 8),
            static_cast<uint8_t>(temperatureTenths & 0xFF),
            fan, 0, 0, 0, 0, 0}};
        (void)xQueueOverwrite(canQueue, &message);

        if (!faultActive) {
            WatchdogDriver::refresh();
        } else if (WatchdogDriver::hasTimedOut()) {
            HardwareSim::printWatchdogReset();
            PwmDriver::setFanSpeed(100); // Safe-state output during reset.
            WatchdogDriver::reset();
            faultActive = false;
        }

        vTaskDelayUntil(&lastWake, controlPeriod);
    }
}

void CanTask(void*) {
    CanMessage message{};
    for (;;) {
        if (xQueueReceive(canQueue, &message, portMAX_DELAY) == pdPASS) {
            CanDriver::transmit(message);
        }
    }
}
}

bool createApplicationTasks() {
    canQueue = xQueueCreate(1, sizeof(CanMessage));
    if (canQueue == nullptr) return false;

    const BaseType_t controlCreated = xTaskCreate(
        ControlTask, "Control", configMINIMAL_STACK_SIZE * 2, nullptr, 2, nullptr);
    const BaseType_t canCreated = xTaskCreate(
        CanTask, "CAN", configMINIMAL_STACK_SIZE * 2, nullptr, 1, nullptr);
    return controlCreated == pdPASS && canCreated == pdPASS;
}
