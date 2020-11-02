#ifdef _WIN32
#include <string>
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <Dbghelp.h>

#define DBGHELP_DLL			"DbgHelp.dll"
#define F_WRITEDUMP			"MiniDumpWriteDump"

export module Minidump;

std::string gBinaryModule;
const std::string gCrashDumpFilename = "crashdump.mdmp";
unsigned int gDumpType = MiniDumpNormal;


export namespace fibo::MiniDump
{
	std::string getDumpFileName()
	{
		return gCrashDumpFilename;
	}

	void monitoring(const std::string& binDir = "", 
		unsigned int dumpType = MiniDumpNormal | MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo);
}

// MiniDumpWriteDump() function declaration (so we can just get the function directly from windows)
typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)
(
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);


namespace fibo::MiniDump
{
	// Purpose: Creates a new file and dumps the exception info into it
	bool writeDump(PEXCEPTION_POINTERS pExceptionInfo, MINIDUMP_TYPE minidumpType)
	{
		// get the function pointer directly so that we don't have to include the .lib, and that
		// we can easily change it to using our own dll when this code is used on win98/ME/2K machines
		HMODULE hDbgHelpDll = ::LoadLibraryA(DBGHELP_DLL);
		if (!hDbgHelpDll) {
			return false;
		}

		BOOL ret = FALSE;
		MINIDUMPWRITEDUMP pfnMiniDumpWrite = (MINIDUMPWRITEDUMP) ::GetProcAddress(hDbgHelpDll, F_WRITEDUMP);

		if (pfnMiniDumpWrite)
		{
			std::string outFilename{ gBinaryModule + gCrashDumpFilename };
			HANDLE hFile = ::CreateFileA(outFilename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile)
			{
				// dump the exception information into the file
				PMINIDUMP_EXCEPTION_INFORMATION pExInfo = new MINIDUMP_EXCEPTION_INFORMATION;
				pExInfo->ThreadId = ::GetCurrentThreadId();
				pExInfo->ExceptionPointers = pExceptionInfo;
				pExInfo->ClientPointers = FALSE;

				ret = (*pfnMiniDumpWrite)(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, minidumpType, pExInfo, NULL, NULL);
				::CloseHandle(hFile);
				delete pExInfo;
			}
		}

		::FreeLibrary(hDbgHelpDll);
		return (ret == TRUE);
	}

	long WINAPI unhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
	{
		// First try to write it with all the indirectly referenced memory (ie: a large file).
		// If that doesn't work, then write a smaller one.
		writeDump(pExceptionInfo, (MINIDUMP_TYPE)gDumpType);
		return 0;
	}

	void monitoring(const std::string& binDir, unsigned int dumpType)
	{
		gBinaryModule = binDir;
		gDumpType = dumpType;
		::SetUnhandledExceptionFilter(unhandledExceptionFilter);
	}
}
#endif