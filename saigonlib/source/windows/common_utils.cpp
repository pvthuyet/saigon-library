module;

#include "config.hpp"
#ifdef SAIGON_OS_WINNT
#include <string>
#include <vector>
#include <windows.h>
#include <array>

module Saigon.CommonUtils;

namespace saigon
{
	std::vector<std::wstring> enumerate_drives()
	{
		std::vector<std::wstring> drives;
		std::array letter = { L'A', L':', L'\\', L'\0' };

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
				auto type = ::GetDriveTypeW(letter.data());
				switch (type) {
				case DRIVE_REMOVABLE:
				case DRIVE_FIXED:
					drives.emplace_back(letter.data());
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
#endif // SAIGON_OS_WINNT