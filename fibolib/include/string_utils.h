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
		* @param rexToken: Regular expression token string format. Ex: whitespace: \\s+
		* @exception: std::exception
		*/
		template<typename TString, 
			typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<TString>>::value
			|| std::is_same<std::wstring, typename std::decay_t<TString>>::value>,
			typename Regex = std::conditional_t<std::is_same<std::string, typename std::decay_t<TString>>::value, std::regex, std::wregex>,
			typename RegexTokenIt = std::regex_token_iterator<typename TString::const_iterator>
		>
		_NODISCARD static std::vector<TString> split(const TString& s, const TString& rexToken)
		{
			if (s.empty()) {
				return std::vector<TString>{};
			}

			// Invalid parameter
			assert(!rexToken.empty());

			std::vector<TString> result;
			const Regex rex(rexToken);
			std::copy(RegexTokenIt(std::cbegin(s), std::cend(s), rex, -1),
				RegexTokenIt(),
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
		* search a sub string in a string
		*
		* @param s : The string search. Accept only std::string or std::wstring
		* @param ss: sub string
		* @exception: std::exception
		* @return: position of ss in s if found. Otherwise npos
		*/
		template<typename TString, typename = typename std::enable_if_t<
			std::is_same<std::string, typename std::decay_t<TString>>::value
			|| std::is_same<std::wstring, typename std::decay_t<TString>>::value>
		>
		std::size_t search(const TString& s, const TString& ss, bool icase = false, const std::locale& loc = std::locale())
		{
			if (std::size(s) < std::size(ss))
			{
				return TString::npos;
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

	namespace StringUtils____
	{

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
	}
}