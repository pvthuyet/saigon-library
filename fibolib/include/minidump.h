#pragma once

#include <string>

namespace fibo::MiniDump
{
	std::string getDumpFileName();
	void monitoring(const std::string& binDir = "", unsigned int dumpType = 7);
}