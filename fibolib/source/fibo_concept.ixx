#include <string>
#include <regex>
#include <concepts>

export module FiboConcept;

export namespace fibo
{
	template<typename T>
	using RootType = std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>;

	template <class T, class... Ts>
	inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>; // true if and only if _Ty is in Ts

	// Character type *****************************************
	template<class T>
	inline constexpr bool is_character_v = is_any_of_v<RootType<T>,
		char, 
		wchar_t,
		char8_t,
		char16_t,
		char32_t
	>;

	template<bool Test, typename TChar>
	struct CharElement {
		using CharacterElement = RootType<TChar>;
	};

	template<typename TChar> requires is_character_v<TChar>
	using CharacterElement = RootType<TChar>;

	// String type ********************************************
	template<class T>
	inline constexpr bool is_string_v = is_any_of_v<std::decay_t<T>, 
		std::string, 
		std::wstring,
		std::u8string,
		std::u16string,
		std::u32string,
		std::string_view,
		std::wstring_view,
		std::u8string_view,
		std::u16string_view,
		std::u32string_view
	>;

	template<typename TString> requires is_string_v<TString>
	using StringElement = typename TString::value_type;

	// String able *******************************************
	template<typename T>
	concept Stringable = requires(T t) {
		{ t } -> std::convertible_to<std::string_view>;
	} or requires(T t) {
		{ t } -> std::convertible_to<std::wstring_view>;
	} or requires(T t) {
		{ t } -> std::convertible_to<std::u8string_view>;
	} or requires(T t) {
		{ t } -> std::convertible_to<std::u16string_view>;
	} or requires(T t) {
		{ t } -> std::convertible_to<std::u32string_view>;
	};

	template<typename T, typename U>
	concept StringablePair = requires(T t, U u) {
		{ t } -> std::convertible_to<std::string_view>;
		{ u } -> std::convertible_to<std::string_view>;
	} or requires(T t, U u) {
		{ t } -> std::convertible_to<std::wstring_view>;
		{ u } -> std::convertible_to<std::wstring_view>;
	} or requires(T t, U u) {
		{ t } -> std::convertible_to<std::u8string_view>;
		{ u } -> std::convertible_to<std::u8string_view>;
	} or requires(T t, U u) {
		{ t } -> std::convertible_to<std::u16string_view>;
		{ u } -> std::convertible_to<std::u16string_view>;
	} or requires(T t, U u) {
		{ t } -> std::convertible_to<std::u32string_view>;
		{ u } -> std::convertible_to<std::u32string_view>;
	};

	template<Stringable TString>
	using TRegex_t = std::conditional_t<
		is_string_v<TString>, 
		std::basic_regex<StringElement<TString>>,
		std::basic_regex<CharacterElement<TString>>
	>;

	template<typename TString> requires is_string_v<TString>
	using TRegexTokenIt_t = std::regex_token_iterator<typename TString::const_iterator>;

	template<Stringable TString>
	using TString_t = std::conditional_t<
		is_string_v<TString>,
		std::basic_string<StringElement<TString>>,
		std::basic_string<CharacterElement<TString>>
	>;

	template<Stringable TString>
	using TStringView_t = std::conditional_t<
		is_string_v<TString>,
		std::basic_string_view<StringElement<TString>>,
		std::basic_string_view<CharacterElement<TString>>
	>;
}