module;

#include "logger_define.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"
#include <memory>

module Fibo.Logger;

namespace fibo
{
	Logger& Logger::createLogger()
	{
		static Logger logger{};
		return logger;
	}

	Logger::Logger()
	{
		initialze();
	}

	void Logger::initialze()
	{
		spdlog::init_thread_pool(8192, 1);
		auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
		auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logger.log", 1024 * 1024 * 10, 10);
		std::vector<spdlog::sink_ptr> sinks{ stdout_sink, rotating_sink };
		auto logger = std::make_shared<spdlog::async_logger>("Fibo", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
		logger->set_pattern("[%Y-%m-%d_%H:%M:%S %e] [%^%l%$] [thread %t] [%!:%#] %v");
		spdlog::set_default_logger(logger);
		spdlog::flush_every(std::chrono::seconds(10));
#ifdef _DEBUG
		spdlog::set_level(spdlog::level::debug);
		spdlog::flush_on(spdlog::level::debug);
		spdlog::flush_every(std::chrono::seconds(1));
#endif // _DEBUG
	}
}