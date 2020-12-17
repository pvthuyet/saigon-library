module;

#include <vector>
#include <string>

export module Saigon.Clipboard;

namespace saigon::clipboard
{
	export std::vector<std::wstring> getCopyingFiles();
}