#pragma once

#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <regex>

namespace fibo
{
	class StringUtils
	{
	public:
		/**
		* Maps a UTF-16 (wide character) string to a new character string
		*
		* @exception: runtime_error
		*/
		_NODISCARD static std::string wc2mb(std::wstring_view str, unsigned int codePage = 65001); // 65001: CP_UTF8

		/**
		* Maps a character string to a UTF-16 (wide character) string
		*
		* @exception: runtime_error
		*/
		_NODISCARD static std::wstring mb2wc(std::string_view str, unsigned int codePage = 65001); // 65001: CP_UTF8

		/**
		* Random an alphabet string from 0-9a-zA-Z
		*
		* @exception: noexcept
		*/
		_NODISCARD std::string randAlphabetString(unsigned len) noexcept;

		/**
		* Parse a string by token
		*
		* @param s : The string is parsed. Accept only std::string or std::wstring
		* @param rexToken: Regular expression token string format
		* @exception: std::exception
		*/
		template<typename T, 
			typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<T>>::value
			|| std::is_same<std::wstring, typename std::decay_t<T>>::value>,
			typename Regex = std::conditional_t<std::is_same<std::string, typename std::decay_t<T>>::value, std::regex, std::wregex>,
			typename RegexTokenIt = std::regex_token_iterator<typename T::const_iterator>
		>
		_NODISCARD static std::vector<T> split(const T& s, const T& rexToken) // whitespace: \\s+
		{
			if (s.empty()) {
				return std::vector<T>{};
			}

			// Invalid parameter
			assert(!rexToken.empty());

			std::vector<T> result;
			const Regex rex(rexToken);
			std::copy(RegexTokenIt(std::cbegin(s), std::cend(s), rex, -1),
				RegexTokenIt(),
				std::back_inserter(result));
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