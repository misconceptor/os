#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <tchar.h>
using namespace std;
vector<string> split(const string& str, const string& delims){
    vector<string> tokens;
    string::size_type start=0;
    string::size_type pos;
    while ((pos=str.find_first_of(delims, start)) != string::npos){
        if(pos > start){
            tokens.push_back(str.substr(start, pos - start));
        }
        start=pos + 1;
    }
    if(start < str.length()){
        tokens.push_back(str.substr(start));
    }
    return tokens;
}
string wstring_to_utf8(const wstring& wstr){
    if(wstr.empty()) return "";
    int size_needed=WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if(size_needed <= 0) return "";
    string result(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
    return result;
}
string tchar_to_string(const TCHAR* tstr){
#ifdef UNICODE
    return wstring_to_utf8(wstring(tstr));
#else
    return string(tstr);
#endif
}
bool kill_process_by_pid(DWORD pid){
    if(pid == 0) return false;
    HANDLE hProcess=OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if(hProcess == nullptr){
        DWORD err=GetLastError();
        cerr<<"OpenProcess failed for PID "<<pid<<", error: "<<err<<"\n";
        return false;
    }
    BOOL result=TerminateProcess(hProcess, 1);
    DWORD err=GetLastError();
    CloseHandle(hProcess);
    if(!result){
        cerr<<"TerminateProcess failed for PID "<<pid<<", error: "<<err<<"\n";
        return false;
    }
    cout<<"Killed process with PID "<<pid<<"\n";
    return true;
}
bool kill_processes_by_name(const string& name){
    bool killed_any=false;
    HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE){
        cerr<<"CreateToolhelp32Snapshot failed\n";
        return false;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize=sizeof(PROCESSENTRY32);
    if(Process32First(hSnapshot, &pe32)){
        do{
            string exe_name=tchar_to_string(pe32.szExeFile);
            if(_stricmp(exe_name.c_str(), name.c_str()) == 0){
                if(kill_process_by_pid(pe32.th32ProcessID)){
                    killed_any=true;
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    if(killed_any){
        cout<<"Killed all processes named '"<<name<<"'\n";
    }
    return killed_any;
}
bool parse_args(int argc, char* argv[], DWORD& target_pid, string& target_name){
    target_pid=0;
    target_name.clear();

    for (int i=1; i < argc; ++i){
        string arg=argv[i];

        if(arg == "--id" && i + 1 < argc){
            char* end;
            unsigned long pid=strtoul(argv[++i], &end, 10);
            if(*end == '\0' && pid > 0 && pid <= 0xFFFFFFFFUL){
                target_pid=static_cast<DWORD>(pid);
            } else{
                cerr<<"Invalid PID: "<<argv[i]<<"\n";
                return false;
            }
        } else if(arg == "--name" && i + 1 < argc){
            target_name=argv[++i];
        } else{
            cerr<<"Unknown option or missing argument: "<<arg<<"\n";
            return false;
        }
    }
    return true;
}
void kill_from_env(){
    char* env_value=getenv("PROC_TO_KILL");
    if(!env_value || strlen(env_value) == 0){
        return;
    }
    string env_str(env_value);
    vector<string> names=split(env_str, ",;");
    for (const string& name : names){
        if(!name.empty()){
            kill_processes_by_name(name);
        }
    }
}
int main(int argc, char* argv[]){
    kill_from_env();
    DWORD target_pid=0;
    string target_name;
    if(!parse_args(argc, argv, target_pid, target_name)){
        cerr<<"Usage: "<<argv[0]<<" [--id <PID>] [--name <process-name>]\n";
        return 1;
    }
    if(target_pid != 0){
        if(!kill_process_by_pid(target_pid)){
            cerr<<"Failed to kill process with PID "<<target_pid<<"\n";
            return 1;
        }
    }
    if(!target_name.empty()){
        if(!kill_processes_by_name(target_name)){
            cerr<<"No process found with name '"<<target_name<<"' or failed to kill\n";
            return 1;
        }
    }
    if(target_pid == 0 && target_name.empty()){
        cout<<"No --id or --name specified. Only killed processes from PROC_TO_KILL.\n";
    }
    return 0;
}