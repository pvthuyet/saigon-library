module;

export module Saigon.Logger;

namespace saigon
{
	export class logger
	{
	public:
		static logger& create_logger();
		logger(logger const&) = delete;
		logger& operator=(logger const&) = delete;

	private:
		logger();
		void initialze();
	};
}