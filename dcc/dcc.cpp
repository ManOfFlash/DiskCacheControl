#include "stdafx.h"

void PrintUsage()
{
    wprintf(L"Control integrated disk write cache. Administrator rights strictly required!\n"
		    L"Usage: dcc Diskletter: on|off(case sensitive!)\n"
            L"Example: dcc C: on\n");
}

BOOLEAN GetCacheState(HANDLE hDevice)
{
    DISK_CACHE_INFORMATION cache_info;
    DWORD buf_size;
    DWORD R = DeviceIoControl(hDevice, IOCTL_DISK_GET_CACHE_INFORMATION, NULL,
                               0, (LPVOID)&cache_info, (DWORD)sizeof(cache_info), (LPDWORD)&buf_size, (LPOVERLAPPED)NULL);
    if (!R)
    {
        errno = GetLastError();
        _wperror(L"Can not get disk cache infiormation!");
        exit(12);
    }
    return cache_info.WriteCacheEnabled;
}

HANDLE OpenDevice(wchar_t* path, bool ReadOnly)
{
    DWORD WRITE_FLAG1 = ReadOnly? 0 : GENERIC_WRITE;
    HANDLE hDevice =
        CreateFile(path,
                   GENERIC_READ | WRITE_FLAG1,
                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL, // security attributes
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_SYSTEM,
                   NULL); // no template file handle
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        errno = GetLastError();
        _wperror(L"Can not open desired device! Are you admin?");
        exit(11);
    }
    return hDevice;
}

int wmain(int argc, wchar_t*argv[])
{
    DWORD R;
    HANDLE hDevice;
    DWORD buf_size;
    bool ReadOnly; //open device in read-only mode
    wchar_t Volume[32];
    wchar_t *Pre = L"\\\\.\\";
    BOOLEAN WriteCacheDesiredState = false;
    if ( (argc < 2)|(argc > 3) ) //we accept 1 or 2 user args. ([0]-always provided by system)
    {
        PrintUsage();
        return(1);
    }
    wcscpy_s(Volume, Pre);     // "\\.\" +
    wcscat_s(Volume, argv[1]); // "C:"= "\\.\C:"

    if (argc == 3)
    {
        ReadOnly = false;
        if (wcscmp(argv[2], L"on") == 0) WriteCacheDesiredState = true;
        else if ( wcscmp(argv[2], L"off") == 0) WriteCacheDesiredState = false;
        else // 2-nd parameter nor on, nor off
        {
            PrintUsage();
            _wperror(L"Malformed parameters!");
            exit(2);
        }
    }
    else ReadOnly = true;

    hDevice = OpenDevice(Volume, ReadOnly);

    BOOLEAN CacheState = GetCacheState(hDevice);
    if (argc == 2) //user only wants findout WC state
    {
        wchar_t *status;
        CacheState = GetCacheState(hDevice);
        if (CacheState == TRUE) status = L"enabled";
        else status = L"disabled";
        wprintf(L"Cache status on device: %s", status);
        return 0;
    }

    if (CacheState == WriteCacheDesiredState)
    {
        wprintf(L"Disk cache already has desired state");
        return 0;
    }

	DISK_CACHE_INFORMATION cache_info;
    cache_info.WriteCacheEnabled = WriteCacheDesiredState;

    R = DeviceIoControl(hDevice, IOCTL_DISK_SET_CACHE_INFORMATION,
                         (LPVOID)&cache_info, (DWORD)sizeof(cache_info),
                         NULL, 0, (LPDWORD)&buf_size, (LPOVERLAPPED)NULL);
    if (!R)
    {
        errno = GetLastError();
        _wperror(L"Can not set disk cache!");
        return 13;
    }

    if (GetCacheState(hDevice) != WriteCacheDesiredState)
    {
        _wperror(L"Disk write cache setting silently ignored by system!");
        return 3;
    }
    CloseHandle(hDevice);
    wprintf(L"OK");
    return 0;
}
