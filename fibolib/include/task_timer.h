#pragma once

#include <atomic>
#include <mutex>
#include <future>
#include <condition_variable>

namespace fibo
{
	enum class TimerStatus
	{
		TIMER_CONTINUE,
		TIMER_FINISH
	};

	class TaskTimer
	{
	public:
		TaskTimer(unsigned long interval = 1000) noexcept; // in milli-second
		virtual ~TaskTimer() noexcept {};

		TaskTimer(const TaskTimer&) = delete;
		TaskTimer& operator=(const TaskTimer&) = delete;

		virtual bool startTimer() noexcept;
		virtual void stopTimer() noexcept;

	protected:
		virtual TimerStatus onTimer() = 0;

	private:
		void execute();

	private:
		unsigned long mInterval;
		std::atomic_bool mStop;
		std::mutex mMuxThread;
		std::condition_variable mCV;
		std::future<void> mFut;
	};
}
