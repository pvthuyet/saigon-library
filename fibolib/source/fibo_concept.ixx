#include <string>
#include <regex>
#include <concepts>

export module FiboConcept;

export namespace fibo
{
	template <class _Ty, class... _Types>
	inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<_Ty, _Types>...>; // true if and only if _Ty is in _Types

	//++ TODO: can't detect wchar_t[]
	template<class _Ty>
	inline constexpr bool is_wide_string_v = is_any_of_v<std::remove_cvref_t<_Ty>, std::wstring, std::wstring_view, wchar_t, wchar_t*, wchar_t const*>;

	template<typename T, typename U>
	concept TStringParam =
		requires(T t, U u) {
			{ t } -> std::convertible_to<std::string_view>;
			{ u } -> std::convertible_to<std::string_view>;
	} or requires(T t, U u) {
		{ t } -> std::convertible_to<std::wstring_view>;
		{ u } -> std::convertible_to<std::wstring_view>;
	};

	template<typename T>
	using TRegex_t = std::conditional_t<std::is_convertible_v<T, std::wstring_view>, std::wregex, std::regex>;

	template<typename T>
	using TRegexTokenIt_t = std::regex_token_iterator<typename T::const_iterator>;

	template<typename T>
	using TString_t = std::conditional_t<std::is_convertible_v<T, std::wstring_view>, std::wstring, std::string>;

	template<typename T>
	using TStringView_t = std::conditional_t<std::is_convertible_v<T, std::wstring_view>, std::wstring_view, std::string_view>;
}