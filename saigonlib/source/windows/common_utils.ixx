module;

#include <string>
#include <vector>
#include <Windows.h>

export module Saigon.CommonUtils;

namespace saigon
{
	export std::vector<std::wstring> enumerate_drives()
	{
		std::vector<std::wstring> drives;
		wchar_t letter[] = { L'A', L':', L'\\', L'\0' };

		// Get all drives in the system.
		DWORD dwDriveMask = ::GetLogicalDrives();

		if (0 == dwDriveMask) {
			return drives;
		}

		// Loop for all drives (MAX_DRIVES = 26)
		constexpr short MAX_DRIVES = 26;

		for (short i = 0; i < MAX_DRIVES; ++i) {
			// if a drive is present,
			if (dwDriveMask & 1) {
				letter[0] = 'A' + i;
				auto type = ::GetDriveTypeW(letter);
				switch (type) {
				case DRIVE_REMOVABLE:
				case DRIVE_FIXED:
					drives.emplace_back(letter);
					break;

				default:
					break;
				}
			}
			dwDriveMask >>= 1;
		}

		return drives;
	}
}
