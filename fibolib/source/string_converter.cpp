#include "string_converter.h"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

using namespace std;

namespace fibo::StringConverter
{
	string wc2mb(const wchar_t* str, int len, unsigned int codePage)
	{
#ifdef _WIN32
		auto size_needed = ::WideCharToMultiByte(codePage, 0, str, len, NULL, 0, NULL, NULL);
		if (size_needed > 0)
		{
			std::string strTo(size_needed, 0);
			::WideCharToMultiByte(codePage, 0, str, len, &strTo[0], size_needed, NULL, NULL);
			return strTo;
		}
#else
		//++ TODO
#endif
		return string{};
	}

	wstring mb2wc(const char* str, int len, unsigned int codePage)
	{
#ifdef _WIN32
		int size_needed = ::MultiByteToWideChar(codePage, 0, str, len, NULL, 0);
		if (size_needed > 0)
		{
			std::wstring wstrTo(size_needed, 0);
			::MultiByteToWideChar(codePage, 0, str, len, &wstrTo[0], size_needed);
			return wstrTo;
		}
#else
		//++ TODO
#endif
		return wstring{};
	}
}