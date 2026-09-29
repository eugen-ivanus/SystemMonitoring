// console_testing.cpp
#include "SystemInfo.h"
#include <iostream>

// Funcție care rulează testele tale
void runConsoleTests() {
    SystemInfo info = SystemInfoProvider::getSystemInfo();
    std::cout << "Nume procesor: " << info.cpuName << std::endl;
    std::cout << "Numar de nuclee: " << info.logicalCores << std::endl;
    std::cout << "Arhitectura CPU: " << info.cpuArchitecture << std::endl;
    for (const auto &p : info.processes) {
        std::cout << p << std::endl;
    }
}
