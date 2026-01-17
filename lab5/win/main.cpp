#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

HANDLE spawn(const std::string& cmd, HANDLE hIn, HANDLE hOut) {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hIn;
    si.hStdOutput = hOut;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    SetHandleInformation(hIn, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    SetHandleInformation(hOut, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    std::vector<char> cmdBuf(cmd.begin(), cmd.end());
    cmdBuf.push_back('\0');

    if (!CreateProcessA(NULL, cmdBuf.data(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        return NULL;
    }

    SetHandleInformation(hIn, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hOut, HANDLE_FLAG_INHERIT, 0);

    CloseHandle(pi.hThread);
    return pi.hProcess;
}

int main() {
    system("taskkill /F /IM m.exe /IM a.exe /IM p.exe /IM s.exe /T >nul 2>&1");

    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hInR, hInW, h1R, h1W, h2R, h2W, h3R, h3W, hOutR, hOutW;

    CreatePipe(&hInR, &hInW, &sa, 0);
    CreatePipe(&h1R, &h1W, &sa, 0);
    CreatePipe(&h2R, &h2W, &sa, 0);
    CreatePipe(&h3R, &h3W, &sa, 0);
    CreatePipe(&hOutR, &hOutW, &sa, 0);

    SetHandleInformation(hInW, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hOutR, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hInR, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(h1R, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(h2R, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(h3R, HANDLE_FLAG_INHERIT, 0);

    HANDLE hM = spawn("m.exe", hInR, h1W);
    HANDLE hA = spawn("a.exe", h1R, h2W);
    HANDLE hP = spawn("p.exe", h2R, h3W);
    HANDLE hS = spawn("s.exe", h3R, hOutW);

    CloseHandle(hInR);
    CloseHandle(h1R); CloseHandle(h1W);
    CloseHandle(h2R); CloseHandle(h2W);
    CloseHandle(h3R); CloseHandle(h3W);
    CloseHandle(hOutW);

    std::cout << "Enter numbers: ";
    std::string input;
    std::getline(std::cin, input);
    input += "\n";

    DWORD dw;
    WriteFile(hInW, input.c_str(), (DWORD)input.size(), &dw, NULL);
    CloseHandle(hInW);

    

    std::string result;
    char buf[512];
    while (ReadFile(hOutR, buf, sizeof(buf) - 1, &dw, NULL) && dw > 0) {
        buf[dw] = '\0';
        result += buf;
    }

    HANDLE procs[4] = { hM, hA, hP, hS };
    WaitForMultipleObjects(4, procs, TRUE, INFINITE);
    for(int i=0; i<4; i++) CloseHandle(procs[i]);
    CloseHandle(hOutR);

    std::cout << "Answer: " << result << std::endl;
    return 0;
}