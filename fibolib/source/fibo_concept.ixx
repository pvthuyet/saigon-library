#include <string>
#include <regex>
#include <concepts>

export module FiboConcept;

export namespace fibo
{
	template <class T, class... Ts>
	inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>; // true if and only if _Ty is in Ts

	template<class TChar>
	inline constexpr bool is_character_v = is_any_of_v<std::decay_t<TChar>, char, wchar_t>;

	template<class TString>
	inline constexpr bool is_string_v = is_any_of_v<std::decay_t<TString>, std::string, std::string_view, char, char const, char*, char const*>;

	template<class TWString>
	inline constexpr bool is_wide_string_v = is_any_of_v<std::decay_t<TWString>, std::wstring, std::wstring_view, wchar_t, wchar_t const, wchar_t*, wchar_t const*>;

	template<typename T, typename U>
	concept TStringParam =
		requires(T t, U u) {
			{ t } -> std::convertible_to<std::string_view>;
			{ u } -> std::convertible_to<std::string_view>;
	} or requires(T t, U u) {
		{ t } -> std::convertible_to<std::wstring_view>;
		{ u } -> std::convertible_to<std::wstring_view>;
	};

	template<typename TString>
	using TRegex_t = std::conditional_t<std::is_convertible_v<TString, std::wstring_view>, std::wregex, std::regex>;

	template<typename TString>
	using TRegexTokenIt_t = std::regex_token_iterator<typename TString::const_iterator>;

	template<typename TString>
	using TString_t = std::conditional_t<std::is_convertible_v<TString, std::wstring_view>, std::wstring, std::string>;

	template<typename TString>
	using TStringView_t = std::conditional_t<std::is_convertible_v<TString, std::wstring_view>, std::wstring_view, std::string_view>;
}