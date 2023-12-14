
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define SIZE 4096

int main() {
    HANDLE shared_memory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZE, "SharedMemory");
    if (shared_memory == NULL) {
        perror("CreateFileMapping");
        exit(EXIT_FAILURE);
    }

    LPVOID shared_memory_ptr = MapViewOfFile(shared_memory, FILE_MAP_ALL_ACCESS, 0, 0, SIZE);
    if (shared_memory_ptr == NULL) {
        perror("MapViewOfFile");
        CloseHandle(shared_memory);
        exit(EXIT_FAILURE);
    }

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcess(NULL, "child_process.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        perror("CreateProcess");
        UnmapViewOfFile(shared_memory_ptr);
        CloseHandle(shared_memory);
        exit(EXIT_FAILURE);
    }

    // Parent Process
    strcpy((char*)shared_memory_ptr, "Hello, child process!");

    WaitForSingleObject(pi.hProcess, INFINITE);

    UnmapViewOfFile(shared_memory_ptr);
    CloseHandle(shared_memory);

    return 0;
}
