#define _CRT_SECURE_NO_WARNINGS       
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <algorithm> 
#include <iterator>
#include <string>
#include <map>

std::map<DWORD, std::string> GetProcList()
{
    std::map<DWORD, std::string> processes_map;
    HANDLE procsnap;
    procsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(procsnap, &procEntry);
    char name[1024];
    wcstombs(name, (wchar_t*)procEntry.szExeFile, sizeof(procEntry.szExeFile));
    processes_map.insert(std::make_pair(procEntry.th32ProcessID, procEntry.szExeFile));
    while (Process32Next(procsnap, &procEntry)) {
        wcstombs(name, (wchar_t*)procEntry.szExeFile, sizeof(procEntry.szExeFile));
        processes_map.insert(std::make_pair(procEntry.th32ProcessID, procEntry.szExeFile));
    }
    CloseHandle(procsnap);
    return processes_map;
}
void compare(std::map<DWORD, std::string> proc1)
{
    std::map<DWORD, std::string> proc2;
    std::map<DWORD, std::string> different_states_array;
    proc2 = GetProcList();
    if (proc1 == proc2)return;
    set_difference(proc2.begin(), proc2.end(), proc1.begin(), proc1.end(),
        inserter(different_states_array, different_states_array.begin()));
    if (!different_states_array.empty())
    {
        for (auto i : different_states_array)
        {
            std::cout << "Process started: " << i.first << "\nName: " << i.second << "\n";
        }
    }
    different_states_array.clear();
    set_difference(proc1.begin(), proc1.end(), proc2.begin(), proc2.end(),
        inserter(different_states_array, different_states_array.begin()));
    if (!different_states_array.empty())
    {
        for (auto i : different_states_array) {
            std::cout << "Process finished: " << i.first << "\nName: " << i.second << "\n\n";
        }
    }
}
int main()
{
    std::map<DWORD, std::string> proc1 = GetProcList();
    bool running = true;
    while (running)
    {
        compare(proc1);
        proc1 = GetProcList();
        Sleep(50);
    }
    return 0;
}