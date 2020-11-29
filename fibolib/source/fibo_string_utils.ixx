module;

#include "fmt/core.h"
#include "constants.h"
#include "fibo_define.h"
#include <execution>
#include <algorithm>
#include <string>
#include <regex>
#include <random>

import Fibo.Concept;
#ifdef _WIN32
import Fibo.WindowsStringApi;
#else
//++ TODO
#endif // _WIN32

export module Fibo.StringUtils;

namespace fibo::StringUtils
{
	/// <summary>
	/// Maps a UTF-16 (wide character) string to a new character string
	/// </summary>
	/// <param name="str"></param>
	/// <param name="codePage"></param>
	/// <returns></returns>
	export [[nodiscard]] auto convert(std::wstring_view str, unsigned int codePage = F_CP_UTF8)
	{
		return StringApi::wc2mb(str, codePage);
	}

	/// <summary>
	/// Maps a character string to a UTF-16 (wide character) string
	/// </summary>
	/// <param name="str"></param>
	/// <param name="codePage"></param>
	/// <returns></returns>
	export [[nodiscard]] auto convert(std::string_view str, unsigned int codePage = F_CP_UTF8)
	{
		return StringApi::mb2wc(str, codePage);
	}

	/// <summary>
	/// Random an alphabet string from 0-9a-zA-Z
	/// </summary>
	/// <param name="len"></param>
	/// <returns></returns>
	export [[nodiscard]] auto randAlphabet(unsigned len)
	{
		constexpr const char alphabet[] =
		{ '0','1','2','3','4','5','6','7','8','9',
		  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

		if (0 == len) {
			return std::string{};
		}

		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine gen(seed);
		std::uniform_int_distribution<unsigned> dis(0, sizeof(alphabet) - 1);
		std::string str(len, 0);

		for (unsigned i = 0; i < len; ++i) {
			str[i] = alphabet[dis(gen)];
		}
		return str;
	}

	/// <summary>
	/// Parse a string by token
	/// </summary>
	/// <param name="str"></param>
	/// <param name="token"></param>
	/// <returns></returns>
	export template<typename S1, typename S2> requires fibo::StringablePair<S1, S2>
	[[nodiscard]] auto split(const S1& str, const S2& token)
	{
		// Valid nullptr for s1 and s2
		if constexpr (std::is_pointer_v<S1>) {
			if (nullptr == str) {
				throw std::invalid_argument(
					fmt::format("Invalid argument. The parameter is nullptr. {}:{}", 
						__FILE__, 
						__LINE__));
			}
		}

		if constexpr (std::is_pointer_v<S2>) {
			if (nullptr == token) {
				throw std::invalid_argument(
					fmt::format("Invalid argument. The parameter is nullptr. {}:{}", 
						__FILE__, 
						__LINE__));
			}
		}

		using TString = tstring_t<S1>;
		using TStringView = tstring_view_t<S1>;
		TStringView sv{ str };
		TStringView tokv{ token };

		if (sv.empty()) {
			return fipmr::vector<TString>{};
		}

		// Invalid parameter
		auto tokenSize = tokv.size();
		if (0 == tokenSize) {
			return fipmr::vector<TString>{ TString{ str } };
		}

		// count number of token in str
		size_t numToken = 0;
		if (1 == tokenSize) {
			auto ch = tokv.front();
			numToken = std::count(std::execution::seq, std::cbegin(sv), std::cend(sv), ch);
		}
		else {
			//++ TODO
		}

		if (0 == numToken) {
			return fipmr::vector<TString>(1, str);
		}

		fipmr::vector<TString> result;
		result.reserve(numToken + 1);

		using TRegex = tregex_t<S1>;
		using TRegexTokenIt = tregex_token_iterator_t<TStringView>;

		TRegex rex(tokv.data());
		std::copy(TRegexTokenIt(std::cbegin(sv), std::cend(sv), rex, -1),
			TRegexTokenIt(),
			std::back_inserter(result));

		return result;
	}

	export template<typename S1, typename S2> requires fibo::StringablePair<S1, S2>
	[[nodiscard]] constexpr auto equal(const S1& s1, const S2& s2, bool icase = false, const std::locale& loc = std::locale())
	{
		auto s = fmt::format("{}", "equal");
		// Valid nullptr for s1 and s2
		if constexpr (std::is_pointer_v<S1>) {
			if (nullptr == s1) {
				throw std::invalid_argument(
					fmt::format("Invalid argument. The parameter is nullptr. {}:{}", 
						__FILE__, 
						__LINE__));
			}
		}

		if constexpr (std::is_pointer_v<S2>) {
			if (nullptr == s2) {
				throw std::invalid_argument(
					fmt::format("Invalid argument. The parameter is nullptr. {}:{}", 
						__FILE__, 
						__LINE__));
			}
		}

		using TStringView = tstring_view_t<S1>;
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