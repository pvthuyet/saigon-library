#ifdef _WIN32
#include "define.h"
#include "fibo_std.h"

export module Fibo.WindowsStringApi;

export namespace fibo::StringApi
{
	F_NODISCARD std::string wc2mb(std::wstring_view str, unsigned int codePage = CP_UTF8) // 65001: CP_UTF8
	{
		int len = static_cast<int>(str.length());

		// There is no data to convert
		if (0 == len) {
			return std::string{};
		}

		auto numOfChars = ::WideCharToMultiByte(codePage, 0, str.data(), len, NULL, 0, NULL, NULL);
		if (0 == numOfChars)
		{
			throw std::runtime_error(F_EXCEPTION_MESSAGE_WINDOWS(std::string{ "Failed to convert wide char to multibyte" }));
		}

		std::string mstr(numOfChars, 0);
		auto retVal = ::WideCharToMultiByte(codePage, 0, str.data(), len, &mstr[0], numOfChars, NULL, NULL);
		if (0 == retVal)
		{
			throw std::runtime_error(F_EXCEPTION_MESSAGE_WINDOWS(std::string{ "Failed to convert wide char to multibyte" }));
		}
		return mstr;
	}

	F_NODISCARD std::wstring mb2wc(std::string_view str, unsigned int codePage = CP_UTF8)
	{
		int len = static_cast<int>(str.length());

		// There is no data to convert
		if (0 == len) {
			return std::wstring{};
		}

		int numOfWideChars = ::MultiByteToWideChar(codePage, 0, str.data(), len, NULL, 0);
		if (0 == numOfWideChars)
		{
			throw std::runtime_error(F_EXCEPTION_MESSAGE_WINDOWS(std::string{ "Failed to convert multibyte to wide char" }));
		}

		std::wstring wstr(numOfWideChars, 0);
		auto retVal = ::MultiByteToWideChar(codePage, 0, str.data(), len, &wstr[0], numOfWideChars);
		if (0 == retVal)
		{
			throw std::runtime_error(F_EXCEPTION_MESSAGE_WINDOWS(std::string{ "Failed to convert multibyte to wide char" }));
		}
		return wstr;
	}
}
#endif // _WIN32