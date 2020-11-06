#include <string>
#include <regex>
#include <concepts>

export module FiboConcept;

namespace fibo
{
	template<typename T>
	using primitive_t = std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>;

	template <class T, class... Ts>
	inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>; // true if and only if _Ty is in Ts

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

	// String able *******************************************
	export template<typename T>
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

	export template<typename T, typename U>
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

	template<bool Condition, Stringable TString>
	struct PrimitiveString : std::type_identity<typename TString::value_type> {};

	template<Stringable TChar>
	struct PrimitiveString<false, TChar> : std::type_identity<primitive_t<TChar>> {};

	export template<Stringable TString>
	using primitive_string_t = typename PrimitiveString<is_string_v<TString>, TString>::type;

	export template<Stringable TString>
	using t_regex_t = std::basic_regex<primitive_string_t<TString>>;

	export template<typename TString> requires is_string_v<TString>
	using t_regex_token_iterator_t = std::regex_token_iterator<typename TString::const_iterator>;

	export template<Stringable TString>
	using t_string_t = std::basic_string<primitive_string_t<TString>>;

	export template<Stringable TString>
	using t_string_view_t = std::basic_string_view<primitive_string_t<TString>>;
}