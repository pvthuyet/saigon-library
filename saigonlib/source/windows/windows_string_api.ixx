module;

#include <string>

export module Saigon.WindowsStringApi;

namespace saigon::stringapi
{
	export std::string wc2mb(std::wstring_view str, unsigned int codePage); // 65001: CP_UTF8
	export std::wstring mb2wc(std::string_view str, unsigned int codePage);
}