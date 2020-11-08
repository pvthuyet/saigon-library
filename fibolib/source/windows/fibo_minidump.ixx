#include <string>

export module Fibo.Minidump;

export namespace fibo::MiniDump
{
	std::string getDumpFileName();
	void monitoring(std::string bin, unsigned int dumptype = 6144); // MiniDumpNormal | MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo
}