#include "define.h"
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <locale>
#include <algorithm>
#include <regex>

import FiboConcept;
#ifdef _WIN32
import WindowsStringApi;
#define FIBO_CP_UTF8	65001 // CP_UTF8
#else
#define FIBO_CP_UTF8	65001 //++ TODO define for Linux
//++ TODO
#endif // _WIN32

export module FiboStringUtils;

namespace fibo::StringUtils
{
	/// <summary>
	/// Maps a UTF-16 (wide character) string to a new character string
	/// </summary>
	/// <param name="str"></param>
	/// <param name="codePage"></param>
	/// <returns></returns>
	export F_NODISCARD std::string convert(std::wstring_view str, unsigned int codePage = FIBO_CP_UTF8)
	{
		return StringApi::wc2mb(str, codePage);
	}

	/// <summary>
	/// Maps a character string to a UTF-16 (wide character) string
	/// </summary>
	/// <param name="str"></param>
	/// <param name="codePage"></param>
	/// <returns></returns>
	export F_NODISCARD std::wstring convert(std::string_view str, unsigned int codePage = FIBO_CP_UTF8)
	{
		return StringApi::mb2wc(str, codePage);
	}

	/// <summary>
	/// Random an alphabet string from 0-9a-zA-Z
	/// </summary>
	/// <param name="len"></param>
	/// <returns></returns>
	export F_NODISCARD std::string randAlphabet(unsigned len)
	{
		constexpr const char alphabet[] =
		{ '0','1','2','3','4','5','6','7','8','9',
		  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

		if (0 == len) return std::string{};

		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine gen(seed);
		std::uniform_int_distribution<unsigned> dis(0, sizeof(alphabet) - 1);
		std::string str(len, 0);

		for (unsigned i = 0; i < len; ++i)
		{
			str[i] = alphabet[dis(gen)];
		}
		return str;
	}

	/// <summary>
	/// Parse a string by token
	/// </summary>
	/// <param name="s"></param>
	/// <param name="token"></param>
	/// <returns></returns>
	export template<typename SRC, typename TOKEN> requires TStringParam<SRC, TOKEN>
	F_NODISCARD auto split(const SRC& s, const TOKEN& token)
	{
		using TString = TString_t<SRC>;
		using TStringView = TStringView_t<SRC>;
		TStringView sv{ s };
		TStringView tokv{ token };

		if (sv.empty()) return std::vector<TString>{};

		// Invalid parameter
		if (tokv.empty()) return std::vector<TString>{ TString{ s } };

		std::vector<TString> result;

		using TRegex = TRegex_t<SRC>;
		using TRegexTokenIt = TRegexTokenIt_t<TStringView>;

		TRegex rex(tokv.data());
		std::copy(TRegexTokenIt(std::cbegin(sv), std::cend(sv), rex, -1),
			TRegexTokenIt(),
			std::back_inserter(result));

		return result;
	}

	export template<typename TString1, typename TString2> requires TStringParam<TString1, TString2>
	F_NODISCARD bool equal(const TString1& s1, const TString2& s2, bool icase = false, const std::locale& loc = std::locale())
	{
		using TStringView = TStringView_t<TString1>;
		TStringView sv1{ s1 };
		TStringView sv2{ s2 };

		if (std::size(sv1) != std::size(sv2)) {
			return false;
		}

		//++ TODO range
		if (icase) {
			return std::equal(std::cbegin(sv1), 
				std::cend(sv1), 
				std::cbegin(sv2), 
				std::cend(sv2), 
				[&loc](const auto& c1, const auto& c2) {
					return std::toupper(c1, loc) == std::toupper(c2, loc);
				});
		}
		return std::equal(std::cbegin(sv1), 
			std::cend(sv1), 
			std::cbegin(sv2), 
			std::cend(sv2));
	}
}