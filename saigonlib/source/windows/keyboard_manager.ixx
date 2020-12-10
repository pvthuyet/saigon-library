module;

#include <thread>
#include <Windows.h>

export module Saigon.KeyboardManager;

namespace saigon
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