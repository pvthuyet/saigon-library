#pragma once

#define F_NODISCARD		_NODISCARD
#define F_EXCEPTION_MESSAGE(s)	(s + __FILE__ + ":" + std::to_string(__LINE__)) //++ TODO std::source_location