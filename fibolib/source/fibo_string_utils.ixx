#include "define.h"
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <locale>
#include <algorithm>
#include <regex>
#include <type_traits>

using fstring		= std::string;
using fwstring		= std::wstring;
using fstring_view	= std::string_view;
using fwstring_view = std::wstring_view;

template <class _Ty, class... _Types>
inline constexpr bool fibo_is_any_of_v = std::disjunction_v<std::is_same<_Ty, _Types>...>; // true if and only if _Ty is in _Types

template<class _Ty>
inline constexpr bool is_wide_string_v = fibo_is_any_of_v<std::remove_cv_t<_Ty>, fwstring, fwstring_view, wchar_t*>;

template<typename T>
concept StringType =	requires(T s) { {s} -> std::convertible_to<fstring_view>; }
						|| requires(T s) { {s} -> std::convertible_to<fwstring_view>; };

template<StringType _Ty>
using TRegex = std::conditional_t<is_wide_string_v<_Ty>, std::wregex, std::regex>;

template<StringType _Ty>
using TRegexTokenIt = std::regex_token_iterator<typename _Ty::const_iterator>;

template<StringType _Ty>
using ReturnStringType = std::conditional_t<is_wide_string_v<_Ty>, fwstring, fstring>;

template<StringType _Ty>
using StringViewType = std::conditional_t<is_wide_string_v<_Ty>, fwstring_view, fstring_view>;

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
	/// <param name="rexToken"></param>
	/// <returns></returns>
	export template<StringType _Ty>
	F_NODISCARD auto split(const _Ty& str, const _Ty& token)
	{
		using ReturnType = ReturnStringType<_Ty>;
		using SV = StringViewType<_Ty>;
		SV src{ str };
		SV token_sv{ token };

		if (src.empty()) return std::vector<ReturnType>{};

		// Invalid parameter
		if (token_sv.empty()) return std::vector<ReturnType>{ ReturnType{ str } };

		std::vector<ReturnType> result;

		TRegex<_Ty> rex(token_sv.data());
		std::copy(TRegexTokenIt<SV>(std::cbegin(src), std::cend(src), rex, -1),
			TRegexTokenIt<SV>(),
			std::back_inserter(result));

		return result;
	}
}