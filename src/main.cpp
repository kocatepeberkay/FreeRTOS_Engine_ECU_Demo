#include "Drivers.hpp"
#include "HardwareSim.hpp"
#include "Tasks.hpp"

#include <cstdlib>
#include <iostream>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

int main() {
    HardwareSim::init();
    HardwareSim::waitForStart();

    AdcDriver::init();
    PwmDriver::init();
    CanDriver::init();
    WatchdogDriver::init(10000);

    if (!createApplicationTasks()) {
        std::cerr << "Failed to create FreeRTOS queue/tasks.\n";
        return EXIT_FAILURE;
    }

    vTaskStartScheduler();
    std::cerr << "FreeRTOS scheduler stopped unexpectedly.\n";
    return EXIT_FAILURE;
}
