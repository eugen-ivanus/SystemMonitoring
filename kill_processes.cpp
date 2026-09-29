#include "kill_processes.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <tlhelp32.h>
std::string wstringToString(const WCHAR* wstr)
{
    int size_needed = WideCharToMultiByte(
        CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

    std::string str(size_needed, 0);

    WideCharToMultiByte(
        CP_UTF8, 0, wstr, -1, &str[0], size_needed, NULL, NULL);

    return str;
}
void killProcess(const std::string &processName) {

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    if (Process32First(snapshot, &pe)) {

        do {
            if (processName == wstringToString(pe.szExeFile)) {

                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);

                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    CloseHandle(snapshot);
                    return;
                }
            }

        } while (Process32Next(snapshot, &pe));
    }

    CloseHandle(snapshot);
}