#pragma once

namespace fibo
{
	class Logger
	{
	public:
		static Logger& createLogger();

	private:
		Logger();
		void initialze();
	};
}