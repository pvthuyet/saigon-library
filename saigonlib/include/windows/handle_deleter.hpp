#ifndef HANDLE_DELETER_HPP_
#define HANDLE_DELETER_HPP_

#pragma once
#include <Windows.h>

namespace saigon
{
	struct CloseHandleDeleter
	{
		void operator()(HANDLE hObj) const noexcept
		{
			::CloseHandle(hObj);
		}
	};

	struct DeleteObjectDeleter
	{
		void operator()(HGDIOBJ hObj) const noexcept
		{
			::DeleteObject(hObj);
		}
	};

	struct RegCloseKeyDeleter
	{
		void operator()(HKEY hObj) const noexcept
		{
			::RegCloseKey(hObj);
		}
	};

	struct CloseFileDeleter
	{
		void operator()(FILE* hObj) const noexcept
		{
			::fclose(hObj);
		}
	};

	struct CloseSocketDeleter
	{
		void operator()(SOCKET hObj) const noexcept
		{
			::closesocket(hObj);
		}
	};

	struct UnmapViewOfFileDeleter
	{
		void operator()(LPVOID hObj) const noexcept
		{
			::UnmapViewOfFile(hObj);
		}
	};

	struct FreeLibraryDeleter
	{
		void operator()(HMODULE hObj) const noexcept
		{
			::FreeLibrary(hObj);
		}
	};

	struct FindCloseDeleter
	{
		void operator()(HANDLE hObj) const noexcept
		{
			::FindClose(hObj);
		}
	};
}

#endif // HANDLE_DELETER_HPP_