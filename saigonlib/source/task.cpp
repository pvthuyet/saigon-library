#include "task.h"

namespace fibo
{
	bool Task::startTask() noexcept
	{
		try
		{
			std::lock_guard<std::mutex> lk(mMux);
			if (mFut.valid()) // Already run
				return true;

			// Create new thread
			mStop.store(false, std::memory_order::memory_order_relaxed);
			mFut = std::async(std::launch::async, &Task::onRun, this);
			return true;
		}
		catch (...)
		{
		}

		return false;
	}

	void Task::stopTask() noexcept
	{
		try
		{
			// Already stopped
			if (mStop.load(std::memory_order::memory_order_relaxed)) {
				return;
			}

			// Stop worker thread
			{
				std::lock_guard<std::mutex> lk(mMux);
				mStop.store(true, std::memory_order::memory_order_relaxed);
			}
			mCV.notify_all();
			if (mFut.valid())
				mFut.get();
		}
		catch(...)
		{ }
	}
}
