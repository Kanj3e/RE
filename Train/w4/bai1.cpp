#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

int main() {
    // snapshot
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cerr << "Lỗi khi tạo snapshot!" << endl;
        return 1;
    }

    // cautruc
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // lietke
    if (!Process32First(hProcessSnap, &pe32)) {
        cerr << "Lỗi khi lấy thông tin tiến trình đầu tiên!" << endl;
        CloseHandle(hProcessSnap);
        return 1;
    }

    cout << "Image Name\t\t\t\t\t\tPID\n";
    cout << "----------------------------------\n";

    do {
        // in
        wcout << pe32.szExeFile << "\t\t\t\t\t\t" << pe32.th32ProcessID << endl;
    } while (Process32Next(hProcessSnap, &pe32));

    // dong handle
    CloseHandle(hProcessSnap);
    return 0;
}

