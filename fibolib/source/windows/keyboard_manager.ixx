module;

#include "jthread/jthread.hpp"
#include <Windows.h>

export module Fibo.KeyboardManager;

namespace fibo
{
	export class KeyboardManager
	{
	public:
		void wait() const
		{
			std::jthread kbEvent([](){
				for (;;) {
					if (::GetKeyState(VK_ESCAPE) & 0x8000) {
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}});
		}
	};
}