#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

bool kill_process_by_pid(DWORD pid);
bool kill_processes_by_name(const string& name);
string tchar_to_string(const TCHAR* tstr);

void list_processes(const string& name) {
    cout << "\n=== Checking processes: " << name << " ===\n";
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        cout << "Snapshot failed\n";
        return;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    bool found = false;
    if (Process32First(hSnapshot, &pe32)) {
        do {
            string exe_name = tchar_to_string(pe32.szExeFile);
            if (_stricmp(exe_name.c_str(), name.c_str()) == 0) {
                cout << "FOUND: " << exe_name << " (PID: " << pe32.th32ProcessID << ")\n";
                found = true;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    if (!found) cout << "No " << name << " processes running\n";
}
string tchar_to_string(const TCHAR* tstr) {
#ifdef UNICODE
    wstring wstr(tstr);
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) return "";
    string result(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
    return result;
#else
    return string(tstr);
#endif
}
bool launch_process(const string& proc_name) {
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    BOOL success = CreateProcessA(
        NULL, (LPSTR)proc_name.c_str(),
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi
    );
    if (success) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        cout << proc_name << " launched successfully\n";
        return true;
    } else {
        DWORD err = GetLastError();
        cout << "Failed to launch " << proc_name << " (Error: " << err << ")\n";
        return false;
    }
}

int main() {
    cout << "=== Process Killer Test Suite ===\n\n";
    cout << "1. Setting PROC_TO_KILL=notepad.exe,mspaint.exe\n";
    _putenv("PROC_TO_KILL=notepad.exe,mspaint.exe");
    char* env_val = getenv("PROC_TO_KILL");
    cout << "PROC_TO_KILL is now: " << (env_val ? env_val : "NOT SET") << "\n\n";
    
    cout << "2. Launching test processes...\n";
    launch_process("notepad.exe");
    Sleep(2000);
    launch_process("mspaint.exe");
    Sleep(2000);
    
    cout << "\n3. Processes BEFORE killing:\n";
    list_processes("notepad.exe");
    list_processes("mspaint.exe");
    
    cout << "\n4. Running killer.exe...\n";
    int killer_result = system(".\\killer.exe");
    cout << "killer.exe returned: " << killer_result << "\n";
    
    cout << "\n5. Processes AFTER killing:\n";
    list_processes("notepad.exe");
    list_processes("mspaint.exe");
    cout << "\n=== Test Complete ===\n";
    //system("pause");
    return 0;
}