#include "task_timer.h"
namespace fibo
{
	TaskTimer::TaskTimer(unsigned long interval) noexcept
		: mStop{ false },
		mInterval{ interval }
	{
	}

	TaskTimer::~TaskTimer() noexcept
	{
		stopTimer();
	}

	bool TaskTimer::startTimer() noexcept
	{
		try
		{
			std::lock_guard<std::mutex> lk(mMuxThread);
			if (mFut.valid()) // Already run
				return true;

			mStop.store(false, std::memory_order::memory_order_relaxed);
			mFut = std::async(std::launch::async, &TaskTimer::execute, this);
		}
		catch (...)
		{
		}

		return true;
	}

	void TaskTimer::stopTimer() noexcept
	{
		try
		{
			// Already stopped
			if (mStop.load(std::memory_order::memory_order_relaxed)) {
				return;
			}

			// Stop worker thread
			{
				std::lock_guard<std::mutex> lk(mMuxThread);
				mStop.store(true, std::memory_order::memory_order_relaxed);
			}
			mCV.notify_one();
			if (mFut.valid())
			{
				mFut.get(); // to clear result
			}
		}
		catch (...)
		{
		}
	}

	void TaskTimer::execute()
	{
		std::unique_lock<std::mutex> lk(mMuxThread);
		std::atomic_bool& refStop = mStop;
		while (!refStop.load(std::memory_order::memory_order_relaxed))
		{
			if (TimerStatus::TIMER_FINISH == onTimer())
			{
				break;
			}

			mCV.wait_for(lk, std::chrono::milliseconds(mInterval), [&refStop]() {
				return refStop.load(std::memory_order::memory_order_relaxed);
			});
		}
	}
}