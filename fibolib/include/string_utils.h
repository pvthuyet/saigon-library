#pragma once

#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <regex>
#include <fmt/format.h>

namespace fibo
{
	class StringUtils
	{
	public:
		_NODISCARD static std::string wc2mb(std::wstring_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
		_NODISCARD static std::wstring mb2wc(std::string_view str, unsigned int codePage = 65001); // 65001: CP_UTF8

		// Generate random filename
		_NODISCARD std::string randAlphabetString(unsigned len) noexcept;

		template<typename T, typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<T>>::value
			|| std::is_same<std::wstring, typename std::decay_t<T>>::value
			>
		>
		_NODISCARD std::vector<T> split(const T& s, const T& rex) // whitespace: \\s+
		{
			// Invalid parameter
			if (s.empty() || rex.empty())
			{
				std::runtime_error(fmt::format("Invalid paramenter. {}:{}", 
					__FILE__, 
					__LINE__));
			}

			std::vector<T> result;

			// For std::wstring
			if constexpr (std::is_same<std::wstring, typename std::decay_t<T>>::value)
			{
				const std::wregex wrex(rex);
				std::copy(std::wsregex_token_iterator(std::cbegin(s), std::cend(s), wrex, -1),
					std::wsregex_token_iterator(),
					std::back_inserter(result));
			}
			else // for std::string
			{
				const std::regex srex(rex);
				std::copy(std::sregex_token_iterator(std::cbegin(s), std::cend(s), srex, -1),
					std::sregex_token_iterator(),
					std::back_inserter(result));
			}
			return result;
		}

		template<typename T, typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<T>>::value
			|| std::is_same<std::wstring, typename std::decay_t<T>>::value
			>
		>
		_NODISCARD bool regexSearch(const T& s, const T& rex, bool icase = false, const std::locale& loc = std::locale()) noexcept
		{
			// Invalid parameter
			if (s.empty() || rex.empty()) {
				return false;
			}

			std::locale oldLoc = std::locale::global(loc);
			bool ret = false;
			try
			{
				if constexpr (std::is_same<std::wstring, typename std::decay_t<T>>::value)
				{
					std::wsmatch sm;
					std::wregex re(rex, icase ? (std::regex_constants::ECMAScript | std::regex_constants::icase) : std::regex_constants::ECMAScript);
					ret = std::regex_search(s, sm, re);
				}
				else
				{
					std::smatch sm;
					std::regex re(rex, icase ? (std::regex_constants::ECMAScript | std::regex_constants::icase) : std::regex_constants::ECMAScript);
					ret = std::regex_search(s, sm, re);
				}
			}
			catch (...)
			{
			}
			std::locale::global(oldLoc);

			return ret;
		}
	};

	namespace StringUtils____
	{
		template<typename charT>
		struct ICmp
		{
			ICmp(const std::locale& loc) noexcept:
			mLoc(loc)
			{}

			bool operator()(const charT& ch1, const charT& ch2) const noexcept
			{
				return std::toupper(ch1, mLoc) == std::toupper(ch2, mLoc);
			}

		private:
			const std::locale& mLoc;
		};

		template<typename T>
		bool equal(const T& s1, const T& s2, bool icase = false, const std::locale& loc = std::locale())
		{
			if (std::size(s1) != std::size(s2))
			{
				return false;
			}

			if (icase)
			{
				return std::equal(std::cbegin(s1), std::cend(s1), std::cbegin(s2), std::cend(s2), ICmp<typename T::value_type>(loc));
			}

			return std::equal(std::cbegin(s1), std::cend(s1), std::cbegin(s2), std::cend(s2));
		}

		template<typename T>
		std::size_t search(const T& cont, const T& s, bool icase = false, const std::locale& loc = std::locale())
		{
			if (std::size(cont) < std::size(s))
			{
				return T::npos;
			}

			if (icase)
			{
				auto it = std::search(std::cbegin(cont), std::cend(cont), std::cbegin(s), std::cend(s), ICmp<typename T::value_type>(loc));
				return (it != std::cend(cont)) ? std::distance(std::cbegin(cont), it) : T::npos;
			}
			else
			{
				auto it = std::search(std::cbegin(cont), std::cend(cont), std::cbegin(s), std::cend(s));
				return (it != std::cend(cont)) ? std::distance(std::cbegin(cont), it) : T::npos;
			}
		}
		
		bool regexSearch(const std::string& str, const std::string& rex, bool icase = false, const std::locale& loc = std::locale());
		bool regexSearch(const std::wstring& str, const std::wstring& rex, bool icase = false, const std::locale& loc = std::locale());
	}
}