module;

#include "constants.h"
#include <algorithm>
#include <string>
#include <regex>
#include <concepts>
#include <stdexcept>

export module Saigon.StringUtils;

import Saigon.Concepts;

namespace saigon::stringutils
{
	export [[nodiscard]] std::string convert(std::wstring_view str, unsigned int codePage = F_CP_UTF8);
	export [[nodiscard]] std::wstring convert(std::string_view str, unsigned int codePage = F_CP_UTF8);

	export [[nodiscard]] std::string randAlphabet(unsigned len);

	export template<class S, class F> requires(saigon::Stringable<S> && std::predicate<F, typename saigon::tstring_t<S>::value_type>)
	[[nodiscard]] auto split(const S& str, F&& pre, size_t maxElement = 0)
	{
		// Valid nullptr for s1 and s2
		if constexpr (std::is_pointer_v<S>) {
			if (nullptr == str) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
			}
		}

		using TString = tstring_t<S>;
		using TStringView = tstring_view_t<S>;
		TStringView sv{ str };

		std::vector<TString> result;
		if (maxElement > 0) {
			result.reserve(maxElement);
		}

		auto substr = [&result](auto const& s, auto const& first, auto const& last) {
			auto const pos = std::distance(std::cbegin(s), first);
			auto const count = std::distance(first, last);
			if (count > 0) {
				result.emplace_back(s.substr(pos, count));
			}
		};

		auto itNex = std::cbegin(sv);
		for (;;) {
			auto found = std::find_if(itNex, std::cend(sv), pre);
			if (found == std::cend(sv)) {
				break;
			}
			substr(sv, itNex, found);
			itNex = found + 1;
		}
		substr(sv, itNex, std::cend(sv));

		return result;
	}

	export template<typename S1, typename S2> requires saigon::StringablePair<S1, S2>
	[[nodiscard]] auto split_regex(const S1& str, const S2& token, size_t maxElement = 0)
	{
		// Valid nullptr for s1 and s2
		if constexpr (std::is_pointer_v<S1>) {
			if (nullptr == str) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
			}
		}

		if constexpr (std::is_pointer_v<S2>) {
			if (nullptr == token) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
			}
		}

		using TString = tstring_t<S1>;
		using TStringView = tstring_view_t<S1>;
		TStringView sv{ str };
		TStringView tokv{ token };

		std::vector<TString> result;
		if (maxElement > 0) {
			result.reserve(maxElement);
		}

		using TRegex = tregex_t<S1>;
		using TRegexTokenIt = saigon::tregex_token_iterator_t<TStringView>;

		TRegex rex(tokv.data());
		std::copy_if(TRegexTokenIt(std::cbegin(sv), std::cend(sv), rex, -1),
			TRegexTokenIt(),
			std::back_inserter(result),
			[](auto const& it) {
				return std::distance(it.first, it.second) > 0;
			}
		);

		return result;
	}

	export template<typename S1, typename S2> requires saigon::StringablePair<S1, S2>
	[[nodiscard]] constexpr auto equal(const S1& s1, const S2& s2, bool icase = false, const std::locale& loc = std::locale())
	{
		// Valid nullptr for s1 and s2
		if constexpr (std::is_pointer_v<S1>) {
			if (nullptr == s1) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
			}
		}

		if constexpr (std::is_pointer_v<S2>) {
			if (nullptr == s2) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
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

	export template<typename S1, typename S2> requires saigon::StringablePair<S1, S2>
		[[nodiscard]] constexpr auto find(const S1& str, const S2& sub, bool icase = false, const std::locale& loc = std::locale())
	{
		// Valid nullptr for s1 and s2
		if constexpr (std::is_pointer_v<S1>) {
			if (nullptr == str) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
			}
		}

		if constexpr (std::is_pointer_v<S2>) {
			if (nullptr == sub) {
				throw std::invalid_argument("The parameter is null"); //++ TODO
			}
		}

		using TStringView = tstring_view_t<S1>;
		TStringView strv{ str };
		TStringView subv{ sub };

		if (strv.empty() || subv.empty()) {
			return false;
		}

		if (not icase) {
			return TStringView::npos != strv.find(subv);
		}

		return false;
	}
}