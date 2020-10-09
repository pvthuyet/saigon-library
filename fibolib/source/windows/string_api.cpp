#ifdef _WIN32

#include "windows/string_api.h"
#include <Windows.h>
#include <fmt/core.h>

using namespace std;

namespace fibo::WindowsApi
{
	string wc2mb(std::wstring_view str, unsigned int codePage)
	{
		int len = static_cast<int>(str.length());
		auto size_needed = ::WideCharToMultiByte(codePage, 0, str.data(), len, NULL, 0, NULL, NULL);
		if (0 == size_needed)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert wide char to multibyte. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}

		std::string strTo(size_needed, 0);
		auto retVal = ::WideCharToMultiByte(codePage, 0, str.data(), len, &strTo[0], size_needed, NULL, NULL);
		if (0 == retVal)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert wide char to multibyte. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}
		return strTo;
	}

	wstring mb2wc(std::string_view str, unsigned int codePage)
	{
		int len = static_cast<int>(str.length());
		int size_needed = ::MultiByteToWideChar(codePage, 0, str.data(), len, NULL, 0);
		if (0 == size_needed)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert multibyte to wide char. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}

		std::wstring wstrTo(size_needed, 0);
		auto retVal = ::MultiByteToWideChar(codePage, 0, str.data(), len, &wstrTo[0], size_needed);
		if (0 == retVal)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert multibyte to wide char. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}
		return wstrTo;
	}
}
#endif // _WIN32