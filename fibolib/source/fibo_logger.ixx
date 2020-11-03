
export module FiboLogger;

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