#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

using namespace std;

// name sang PID
DWORD FindProcessIDByName(const wstring& processName) {
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cerr << "khong nap dc snapshot!" << endl;
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        cerr << "khong tim thay tien trinh dau tien" << endl;
        CloseHandle(hProcessSnap);
        return 0;
    }

    // Duyệt qua các tiến trình để tìm tên tương ứng
    do {
        // Chuyển đổi từ CHAR[] sang wstring
        wstring exeFileName = wstring(pe32.szExeFile, pe32.szExeFile + strlen(pe32.szExeFile));

        if (exeFileName == processName) {
            CloseHandle(hProcessSnap);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return 0; // khong tim dc
}

// ket thuc bang PID
bool TerminateProcessByID(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess == NULL) {
        cerr << "khong tim thay pid: " << processID << endl;
        return false;
    }

    if (!TerminateProcess(hProcess, 0)) {
        cerr << "Ket thuc khong thanh cong!" << endl;
        CloseHandle(hProcess);
        return false;
    }

    cout << "ket thuc thanh cong (PID) " << processID << endl;
    CloseHandle(hProcess);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "taskkill_clone <method> <name_or_pid>" << endl;
        cout << "-name (bang ten) hoac  -pid (bang PID)" << endl;
        return 1;
    }

    string method = argv[1];
    string target = argv[2];

    if (method == "-pid") {
        DWORD pid = stoi(target);
        TerminateProcessByID(pid);
    }
    else if (method == "-name") {
        wstring processName = wstring(target.begin(), target.end());
        DWORD pid = FindProcessIDByName(processName);
        if (pid != 0) {
            TerminateProcessByID(pid);
        }
        else {
            cout << "khong tim thay voi name :: " << target << endl;
        }
    }
    else {
        cout << "sai phuong thuc, hay su dung: -name hoặc -pid." << endl;
        return 1;
    }

    return 0;
}

