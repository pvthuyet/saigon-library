#ifdef _WIN32

#include "windows/string_api.h"
#include <Windows.h>
#include <fmt/format.h>

using namespace std;

namespace fibo::WindowsApi
{
	string StringApi::wc2mb(std::wstring_view str, unsigned int codePage)
	{
		int len = static_cast<int>(str.length());

		// There is no data to convert
		if (0 == len) {
			return string{};
		}

		auto numOfChars = ::WideCharToMultiByte(codePage, 0, str.data(), len, NULL, 0, NULL, NULL);
		if (0 == numOfChars)
		{
			throw std::runtime_error(fmt::format("Failed to convert wide char to multibyte. Error: {}. {}:{}",
				::GetLastError(),
				__FILE__,
				__LINE__));
		}

		std::string strResult(numOfChars, 0);
		auto retVal = ::WideCharToMultiByte(codePage, 0, str.data(), len, &strResult[0], numOfChars, NULL, NULL);
		if (0 == retVal)
		{
			throw std::runtime_error(fmt::format("Failed to convert wide char to multibyte. Error: {}. {}:{}",
				::GetLastError(),
				__FILE__,
				__LINE__));
		}
		return strResult;
	}

	wstring StringApi::mb2wc(std::string_view str, unsigned int codePage)
	{
		int len = static_cast<int>(str.length());
		
		// There is no data to convert
		if (0 == len) {
			return wstring{};
		}

		int numOfWideChars = ::MultiByteToWideChar(codePage, 0, str.data(), len, NULL, 0);
		if (0 == numOfWideChars)
		{
			throw std::runtime_error(fmt::format("Failed to convert multibyte to wide char. Error: {}. {}:{}",
				::GetLastError(),
				__FILE__,
				__LINE__));
		}

		std::wstring strResult(numOfWideChars, 0);
		auto retVal = ::MultiByteToWideChar(codePage, 0, str.data(), len, &strResult[0], numOfWideChars);
		if (0 == retVal)
		{
			throw std::runtime_error(fmt::format("Failed to convert multibyte to wide char. Error: {}. {}:{}",
				::GetLastError(),
				__FILE__,
				__LINE__));
		}
		return strResult;
	}
}
#endif // _WIN32