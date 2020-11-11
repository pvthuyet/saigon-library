module;

export module Fibo.Logger;

namespace fibo
{
	export class Logger
	{
	public:
		static Logger& createLogger();

	private:
		Logger();
		void initialze();
	};
}