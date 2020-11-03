#include "define.h"
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <locale>
#include <algorithm>
#include <regex>
#include <type_traits>


//// https://stackoverflow.com/questions/51128745/how-can-i-check-if-a-template-type-parameter-is-a-character-type-or-a-string-typ
//template<class T> struct tag_t {};
//template<class T> constexpr tag_t<T> tag{};
//
//template<class T> constexpr std::char_traits<T> tag_char{};
//
//namespace dectect_string {
//	template<class T, class ...Ts>
//	constexpr bool is_stringlike(tag_t<T>, Ts&&...) { return false; }
//	template<class T, class A>
//	constexpr bool is_stringlike(tag_t<std::basic_string<T, A>>) { return true; }
//	template<class T>
//	constexpr bool dectect = is_stringlike(tag<T>);
//}
//
//namespace dectect_charactor {
//	template<class T, class ...Ts>
//	constexpr bool is_characterlike(std::char_traits<T>, Ts&&...) { return false; }
//	template<class T>
//	constexpr bool is_characterlike(std::char_traits<T>) { return true; }
//	template<class T>
//	constexpr bool dectect = is_characterlike(tag_char<T>);
//}


using fstring		= std::string;
using fwstring		= std::wstring;
using fstring_view	= std::string_view;
using fwstring_view = std::wstring_view;

template <class _Ty, class... _Types>
inline constexpr bool fibo_is_any_of_v = std::disjunction_v<std::is_same<_Ty, _Types>...>; // true if and only if _Ty is in _Types

template<class _Ty>
inline constexpr bool is_wide_string_v = fibo_is_any_of_v<std::remove_cv_t<_Ty>, fwstring, fwstring_view, wchar_t, wchar_t[], const wchar_t[], wchar_t*>;

template<class _Ty>
inline constexpr bool is_string_v = fibo_is_any_of_v<std::remove_cv_t<_Ty>, fstring, fstring_view, char, char[], const char[], char*>;

template<class _Ty>
inline constexpr bool is_text_v = is_string_v<_Ty> or is_wide_string_v<_Ty>;

template<typename T>
concept StringType = requires(T s) { {s} -> std::convertible_to<fstring_view>; } or
					 requires(T s) { {s} -> std::convertible_to<fwstring_view>; };

template<typename T>
concept TokenType = std::is_same_v<T, std::string>;

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
	export template<typename _SRC, typename _TOKEN>
	F_NODISCARD auto split(const _SRC& str, const _TOKEN& token)
	{
		static_assert(StringType<_SRC>, "_SRC is not text");
		static_assert(StringType<_TOKEN>, "_TOKEN is not text");
		static_assert(is_wide_string_v<_SRC> == is_wide_string_v<_TOKEN>, "_SRC and _TOKEN are not the same type");

		using ReturnType = ReturnStringType<_SRC>;
		using SV = StringViewType<_SRC>;
		SV src{ str };
		SV token_sv{ token };

		if (src.empty()) return std::vector<ReturnType>{};

		// Invalid parameter
		if (token_sv.empty()) return std::vector<ReturnType>{ ReturnType{ str } };

		std::vector<ReturnType> result;

		TRegex<_SRC> rex(token_sv.data());
		std::copy(TRegexTokenIt<SV>(std::cbegin(src), std::cend(src), rex, -1),
			TRegexTokenIt<SV>(),
			std::back_inserter(result));

		return result;
	}
}