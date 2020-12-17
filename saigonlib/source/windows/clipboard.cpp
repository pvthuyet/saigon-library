module;

#include "windows/scope.hpp"
#include "windows/handle_deleter.hpp"
#include <windows.h>

module Saigon.Clipboard;

namespace saigon::clipboard
{
	std::vector<std::wstring> getCopyingFiles()
	{
		std::vector<std::wstring> filePaths{};

		using std::experimental::make_unique_resource_checked;
		auto cbOpen = make_unique_resource_checked(
			::OpenClipboard(nullptr),
			FALSE,
			[](BOOL) {
				::CloseClipboard();
			});

		if (not cbOpen.get()) return filePaths;
		{
			auto cbData = make_unique_resource_checked(
				::GetClipboardData(CF_HDROP),
				nullptr,
				[](auto) {});

			if (not cbData.get()) return filePaths;
			{
				auto hDrop = make_unique_resource_checked(
					::GlobalLock(cbData.get()),
					nullptr,
					[hData = cbData.get()](auto) {
					::GlobalUnlock(hData);
				});

				if (not hDrop.get()) return filePaths;
				{
					// query files from clipboard
					auto numFiles = ::DragQueryFileW(static_cast<HDROP>(hDrop.get()), 0xFFFFFFFF, nullptr, 0);
					if (not numFiles) return filePaths;

					//++ IMPORTANT: NEVER EVER FORGET RESERVE
					filePaths.reserve(numFiles);

					wchar_t paths[MAX_PATH] = { 0 };
					for (UINT i = 0; i < numFiles; ++i) {
						auto len = ::DragQueryFileW(static_cast<HDROP>(hDrop.get()), i, paths, MAX_PATH);
						if (0 == len) continue;
						filePaths.emplace_back(paths);
					}
				}
			}
		}
		return filePaths;
	}
}