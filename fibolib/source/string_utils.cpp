#include "string_utils.h"
#include <regex>
#include <chrono>
#include <random>
#include <Windows.h>

#if USE_WINDOWS_API
#include "windows/string_api.h"
using OSAPI	= fibo::WindowsApi::StringApi;
#else
using OSAPI = fibo::Standard;
#endif // _WIN32

using namespace std;

namespace fibo
{
	std::string StringUtils::wc2mb(std::wstring_view str, unsigned int codePage)
	{
		return OSAPI::wc2mb(str, codePage);
	}

	std::wstring StringUtils::mb2wc(std::string_view str, unsigned int codePage)
	{
		return OSAPI::mb2wc(str, codePage);
	}

	namespace StringUtils____
	{
		bool regexSearch(const std::string& str, const std::string& rex, bool icase, const std::locale& loc)
		{
			std::locale oldLoc = std::locale::global(loc);
			bool ret = false;
			try
			{
				std::smatch sm;
				std::regex re(rex, icase ? (std::regex_constants::ECMAScript | std::regex_constants::icase) : std::regex_constants::ECMAScript);
				ret = std::regex_search(str, sm, re);
			}
			catch(...)
			{ }
			std::locale::global(oldLoc);

			return ret;
		}

		bool regexSearch(const std::wstring& str, const std::wstring& rex, bool icase, const std::locale& loc)
		{
			std::locale oldLoc = std::locale::global(loc);
			bool ret = false;
			try
			{
				std::wsmatch sm;
				std::wregex re(rex, icase ? (std::regex_constants::ECMAScript | std::regex_constants::icase) : std::regex_constants::ECMAScript);
				ret = std::regex_search(str, sm, re);
			}
			catch (...)
			{
			}
			std::locale::global(oldLoc);

			return ret;
		}

		std::vector<std::wstring> split(const std::wstring& s, const std::wstring& regex)
		{
			std::vector<std::wstring> result;
			try
			{
				const std::wregex ws_re(regex); // whitespace
				std::copy(std::wsregex_token_iterator(s.cbegin(), s.cend(), ws_re, -1),
					std::wsregex_token_iterator(),
					std::back_inserter(result));
			}
			catch(...)
			{ }

			return result;
		}

		std::string randString(unsigned len)
		{
			constexpr const char alphabet[] =
				{ '0','1','2','3','4','5','6','7','8','9',
				  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
				  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
			unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
			std::default_random_engine gen(seed);
			std::uniform_int_distribution<unsigned> dis(0, sizeof(alphabet) - 1);
			std::vector<char> vec(len + 1, '\0');

			for (unsigned i = 0; i < len; ++i)
			{
				vec[i] = alphabet[dis(gen)];
			}

			return std::string{ vec.data() };;
		}
	}
}