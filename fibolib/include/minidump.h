#pragma once
#include <string>

namespace fibo
{
	namespace Minidump
	{
		std::string getDumpFileName();
		void startMonitering(const std::string& binDir, unsigned int dumpType = 7);
	}
}