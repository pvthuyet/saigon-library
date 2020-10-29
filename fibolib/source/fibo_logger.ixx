
export module FiboLogger;

export namespace fibo
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