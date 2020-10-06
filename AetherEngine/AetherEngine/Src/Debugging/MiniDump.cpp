#include "MiniDump.h"

#ifdef AETH_WINDOWS
#include <Windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>
#include <minidumpapiset.h>

namespace Aeth::Debugging {
    // NOTE: There is a linker error in this function stopping the program from being built. MiniDumpWriteDump is unresolved.
    /*int HandleCrash() {
        BOOL bMiniDumpSuccessful;
        WCHAR szPath[MAX_PATH];
        WCHAR szFileName[MAX_PATH];
        WCHAR szAppName = WCHAR(u"AetherEngine");
        WCHAR szVersion = WCHAR(u"v0.1");
        DWORD dwBufferSize = MAX_PATH;
        HANDLE hDumpFile;
        SYSTEMTIME stLocalTime;
        MINIDUMP_EXCEPTION_INFORMATION ExpParam;

        GetLocalTime(&stLocalTime);
        GetTempPath(dwBufferSize, szPath);

        StringCchPrintf(szFileName, MAX_PATH, L"%s%s", szPath, szAppName);
        CreateDirectory(szFileName, NULL);

        StringCchPrintf(szFileName, MAX_PATH, L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
            szPath, szAppName, szVersion,
            stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
            stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
            GetCurrentProcessId(), GetCurrentThreadId());
        hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

        ExpParam.ThreadId = GetCurrentThreadId();
        ExpParam.ClientPointers = TRUE;

        bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
            hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

        return EXCEPTION_EXECUTE_HANDLER;
    }*/
}
#else
namespace Aeth::Debugging {
    int HandleCrash() {

    }
}
#endif