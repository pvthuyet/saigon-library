#pragma once

#include <future>
#include <mutex>

namespace fibo
{
	class Task
	{
	public:
		virtual ~Task() noexcept {};
		Task(const Task&) = delete;
		Task& operator=(const Task&) = delete;

		virtual bool startTask() noexcept;
		virtual void stopTask() noexcept;

	protected:
		virtual void onRun() = 0;

	protected:
		std::atomic_bool mStop{ false };
		std::mutex mMux;
		std::condition_variable mCV;
		std::future<void> mFut;
	};
}

