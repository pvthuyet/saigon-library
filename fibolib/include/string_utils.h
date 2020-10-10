#pragma once

#include <string>
#include <vector>
#include <locale>
#include <algorithm>

namespace fibo
{
	class StringUtils
	{
	public:
		_NODISCARD static std::string wc2mb(std::wstring_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
		_NODISCARD static std::wstring mb2wc(std::string_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
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

		std::vector<std::wstring> split(const std::wstring& s, const std::wstring& regex = L"\\s+");

		// Generate random filename
		std::string randString(unsigned len);
	}
}