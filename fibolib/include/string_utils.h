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
		_NODISCARD static std::string randAlphabetString(unsigned len) noexcept;

		/**
		* Parse a string by token
		*
		* @param s : The string is parsed. Accept only std::string or std::wstring
		* @param rexToken: Regular expression token string format. Ex: whitespace: \\s+
		* @exception: std::exception
		*/
		template<typename TString, 
			typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<TString>>::value
			|| std::is_same<std::wstring, typename std::decay_t<TString>>::value>,
			typename TRegex = std::conditional_t<std::is_same<std::string, typename std::decay_t<TString>>::value, std::regex, std::wregex>,
			typename TRegexTokenIt = std::regex_token_iterator<typename TString::const_iterator>
		>
		_NODISCARD static std::vector<TString> split(const TString& s, const TString& rexToken)
		{
			if (s.empty()) 
			{
				return std::vector<TString>{};
			}

			// Invalid parameter
			if (rexToken.empty())
			{
				return std::vector<TString>{ s };
			}

			std::vector<TString> result;
			const TRegex rex(rexToken);
			std::copy(TRegexTokenIt(std::cbegin(s), std::cend(s), rex, -1),
				TRegexTokenIt(),
				std::back_inserter(result));

			return result;
		}

		/**
		* Compare character with locale
		*/
		template<typename TChar, typename = typename std::enable_if_t<
			std::is_same<char, typename std::decay_t<TChar>>::value
			|| std::is_same<wchar_t, typename std::decay_t<TChar>>::value>
		>
		struct ICmp
		{
			ICmp(const std::locale& loc) noexcept :
				mLoc(loc)
			{}

			bool operator()(const TChar& ch1, const TChar& ch2) const noexcept
			{
				return std::toupper(ch1, mLoc) == std::toupper(ch2, mLoc);
			}

		private:
			const std::locale& mLoc;
		};

		/**
		* Compare to string with locale. Accept only std::string or std::wstring
		*
		* @exception: std::exception
		* @return: true if equal. Othewise false.
		*/
		template<typename TString, typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<TString>>::value
			|| std::is_same<std::wstring, typename std::decay_t<TString>>::value>
		>
		_NODISCARD static bool equal(const TString& s1, const TString& s2, bool icase = false, const std::locale& loc = std::locale())
		{
			if (std::size(s1) != std::size(s2))
			{
				return false;
			}

			if (icase)
			{
				return std::equal(std::cbegin(s1), std::cend(s1), std::cbegin(s2), std::cend(s2), ICmp<typename TString::value_type>(loc));
			}

			return std::equal(std::cbegin(s1), std::cend(s1), std::cbegin(s2), std::cend(s2));
		}

		/**
		* search a sub string in a string. Accept only std::string or std::wstring
		*
		* @param s : The string search.
		* @param ss: sub string
		* @exception: std::exception
		* @return: position of ss in s if found. Otherwise npos
		*/
		template<typename TString, typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<TString>>::value
			|| std::is_same<std::wstring, typename std::decay_t<TString>>::value>
		>
		_NODISCARD static std::size_t search(const TString& s, const TString& ss, bool icase = false, const std::locale& loc = std::locale())
		{
			auto ssSz = std::size(ss);
			if (std::size(s) < ssSz)
			{
				return TString::npos;
			}

			// ss is empty
			if (0 == ssSz)
			{
				return 0;
			}

			if (icase)
			{
				auto it = std::search(std::cbegin(s), std::cend(s), std::cbegin(ss), std::cend(ss), ICmp<typename TString::value_type>(loc));
				return (it != std::cend(s)) ? std::distance(std::cbegin(s), it) : TString::npos;
			}
			else
			{
				auto it = std::search(std::cbegin(s), std::cend(s), std::cbegin(ss), std::cend(ss));
				return (it != std::cend(s)) ? std::distance(std::cbegin(s), it) : TString::npos;
			}
		}

		/**
		* Regular expression search in string
		*
		* @param s : The string search. Accept only std::string or std::wstring
		* @param rex: Regular expression string format.
		* @exception: std::exception
		*/
		template<typename TString, typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<TString>>::value
			|| std::is_same<std::wstring, typename std::decay_t<TString>>::value>,
			typename Regex = std::conditional_t<std::is_same<std::string, typename std::decay_t<TString>>::value, std::regex, std::wregex>
		>
		_NODISCARD static bool regexSearch(const TString& s, const TString& rex, bool icase = false, const std::locale& loc = std::locale()) noexcept
		{
			// Invalid parameter
			if (s.empty() || rex.empty()) {
				return false;
			}

			std::locale oldLoc = std::locale::global(loc);
			bool ret = false;
			try
			{
				Regex re(rex, icase ? (std::regex_constants::ECMAScript | std::regex_constants::icase) : std::regex_constants::ECMAScript);
				ret = std::regex_search(s, re);
			}
			catch (...)
			{
			}
			std::locale::global(oldLoc);

			return ret;
		}
	};
}