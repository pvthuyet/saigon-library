module;

#include <string>

export module Saigon.Minidump;

export namespace saigon::MiniDump
{
	std::string getDumpFileName();
	void monitoring(std::string_view bin, unsigned int dumptype = 6144); // MiniDumpNormal | MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo
}