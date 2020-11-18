module;

#include <concepts>
#include <string>
#include <regex>
#include "fibo_define.h"

export module Fibo.Concept;

namespace fibo
{
	template<typename T>
	using primitive_t = fistd::remove_cvref_t<fistd::remove_pointer_t<fistd::decay_t<T>>>;

	template <class T, class... Ts>
	inline constexpr bool is_any_of_v = fistd::disjunction_v<fistd::is_same<T, Ts>...>; // true if and only if _Ty is in Ts

	// String type ********************************************
	template<class T>
	inline constexpr bool is_string_v = is_any_of_v<fistd::decay_t<T>, 
		fistd::string, 
		fistd::wstring,
		fistd::u8string,
		fistd::u16string,
		fistd::u32string,
		fistdpmr::string,
		fistdpmr::wstring,
		fistdpmr::u8string,
		fistdpmr::u16string,
		fistdpmr::u32string,
		fistd::string_view,
		fistd::wstring_view,
		fistd::u8string_view,
		fistd::u16string_view,
		fistd::u32string_view
	>;

	// String able *******************************************
	export template<typename T> 
	concept Stringable = (not fistd::is_null_pointer_v<T>) and (requires(T t) {
		{ t } -> fistd::convertible_to<fistd::string_view>;
	} or requires(T t) {
		{ t } -> fistd::convertible_to<fistd::wstring_view>;
	} or requires(T t) {
		{ t } -> fistd::convertible_to<fistd::u8string_view>;
	} or requires(T t) {
		{ t } -> fistd::convertible_to<fistd::u16string_view>;
	} or requires(T t) {
		{ t } -> fistd::convertible_to<fistd::u32string_view>;
	});

	export template<typename T, typename U>
	concept StringablePair = (not fistd::is_null_pointer_v<T>) and (not fistd::is_null_pointer_v<U>) and (requires(T t, U u) {
		{ t } -> fistd::convertible_to<fistd::string_view>;
		{ u } -> fistd::convertible_to<fistd::string_view>;
	} or requires(T t, U u) {
		{ t } -> fistd::convertible_to<fistd::wstring_view>;
		{ u } -> fistd::convertible_to<fistd::wstring_view>;
	} or requires(T t, U u) {
		{ t } -> fistd::convertible_to<fistd::u8string_view>;
		{ u } -> fistd::convertible_to<fistd::u8string_view>;
	} or requires(T t, U u) {
		{ t } -> fistd::convertible_to<fistd::u16string_view>;
		{ u } -> fistd::convertible_to<fistd::u16string_view>;
	} or requires(T t, U u) {
		{ t } -> fistd::convertible_to<fistd::u32string_view>;
		{ u } -> fistd::convertible_to<fistd::u32string_view>;
	});

	template<bool Condition, Stringable TString>
	struct PrimitiveString : fistd::type_identity<typename TString::value_type> {};

	template<Stringable TChar>
	struct PrimitiveString<false, TChar> : fistd::type_identity<primitive_t<TChar>> {};

	template<Stringable TString>
	using primitive_string_t = typename PrimitiveString<is_string_v<TString>, TString>::type;

	export template<Stringable TString>
	using tregex_t = fistd::basic_regex<primitive_string_t<TString>>;

	export template<typename TString> requires is_string_v<TString>
	using tregex_token_iterator_t = fistd::regex_token_iterator<typename TString::const_iterator>;

	export template<Stringable TString>
	using tstring_t = fistdpmr::basic_string<primitive_string_t<TString>>;

	export template<Stringable TString>
	using tstring_view_t = fistd::basic_string_view<primitive_string_t<TString>>;
}