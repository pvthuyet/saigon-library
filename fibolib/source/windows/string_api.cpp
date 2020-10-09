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
		auto numOfChars = ::WideCharToMultiByte(codePage, 0, str.data(), len, NULL, 0, NULL, NULL);
		if (0 == numOfChars)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert wide char to multibyte. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}

		std::string strResult(numOfChars, 0);
		auto retVal = ::WideCharToMultiByte(codePage, 0, str.data(), len, &strResult[0], numOfChars, NULL, NULL);
		if (0 == retVal)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert wide char to multibyte. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}
		return strResult;
	}

	wstring mb2wc(std::string_view str, unsigned int codePage)
	{
		int len = static_cast<int>(str.length());
		int numOfWideChars = ::MultiByteToWideChar(codePage, 0, str.data(), len, NULL, 0);
		if (0 == numOfWideChars)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert multibyte to wide char. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}

		std::wstring strResult(numOfWideChars, 0);
		auto retVal = ::MultiByteToWideChar(codePage, 0, str.data(), len, &strResult[0], numOfWideChars);
		if (0 == retVal)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert multibyte to wide char. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}
		return strResult;
	}
}
#endif // _WIN32