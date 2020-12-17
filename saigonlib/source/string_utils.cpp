module;

#include <string>
#include <random>
#include <chrono>

module Saigon.StringUtils;

import Saigon.WindowsStringApi;
namespace saigon::stringutils
{
	std::string convert(std::wstring_view str, unsigned int codePage)
	{
		return stringapi::wc2mb(str, codePage);
	}
	
	std::wstring convert(std::string_view str, unsigned int codePage)
	{
		return stringapi::mb2wc(str, codePage);
	}

	std::string randAlphabet(unsigned len)
	{
		constexpr const char alphabet[] =
		{ '0','1','2','3','4','5','6','7','8','9',
		  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

		if (0 == len) {
			return std::string{};
		}

		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine gen(seed);
		std::uniform_int_distribution<unsigned> dis(0, sizeof(alphabet) - 1);
		std::string str(len, 0);

		for (unsigned i = 0; i < len; ++i) {
			str[i] = alphabet[dis(gen)];
		}
		return str;
	}
}