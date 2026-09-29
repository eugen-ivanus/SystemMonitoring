// SystemInfo.h
#pragma once
#include <windows.h>
#include <vector>
#include <string>
struct SystemInfo {
    std::string cpuName;
    int logicalCores;
    int cpuArchitecture;
    int ramMem;
    int diskMem;
    std::vector<std::string> processes;

};

class SystemInfoProvider {
public:
    static SystemInfo getSystemInfo();
};
