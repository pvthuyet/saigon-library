#pragma once
#ifdef _WIN32

#include <string>

namespace fibo::WindowsApi
{
	std::string getDumpFileName();
	void startMonitering(const std::string& binDir, unsigned int dumpType = 7);
}

#endif // _WIN32