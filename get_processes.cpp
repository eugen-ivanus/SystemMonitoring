#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include "get_processes.h"
std::vector<std::string> getProcesses1() {
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
