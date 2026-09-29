#include "SystemInfo.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <tlhelp32.h>

std::vector<std::string> getProcesses() {
    std::vector<std::string> processes;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
        return processes;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    if (Process32First(snapshot, &pe)) {
        do {
            processes.emplace_back(std::string(pe.szExeFile, pe.szExeFile + wcslen(pe.szExeFile)));
        } while (Process32Next(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return processes;
}


std::string getCPUname() {
    HKEY hKey;
    const char* path = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";

    char cpuName[256];
    DWORD size = sizeof(cpuName);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)cpuName, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::string(cpuName);
        }
        RegCloseKey(hKey);
    }

    return "";
}



SystemInfo SystemInfoProvider::getSystemInfo() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    //RAM MEMORY
      MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    int totram;
    if (GlobalMemoryStatusEx(&memInfo)) {
        DWORDLONG totalRAM = memInfo.ullTotalPhys;

        totram = totalRAM / (1024*1024);

    }

    //Internal Memory

    ULARGE_INTEGER freeBytesAvailable, totalBytes, freeBytes;
    int totalDisk;
    if (GetDiskFreeSpaceEx(TEXT("C:\\"), &freeBytesAvailable, &totalBytes, &freeBytes)) {
        totalDisk = totalBytes.QuadPart / (1024 * 1024);
        int freeDisk = freeBytes.QuadPart / (1024 * 1024);
    }


    SystemInfo info;
    info.cpuName = getCPUname(); //cpu name
    info.logicalCores = static_cast<int>(si.dwNumberOfProcessors);//numbers of cores
    info.cpuArchitecture=static_cast<int>(si.wProcessorArchitecture);//procesor architecture
    info.ramMem=totram; //ram memory
    info.diskMem = totalDisk;//total internal memory
    info.processes = getProcesses();//proceses

    return info;

}
