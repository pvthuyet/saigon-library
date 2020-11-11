#ifndef DEFINE_H_
#define DEFINE_H_

#pragma once

#define F_NODISCARD		_NODISCARD
#define F_EXCEPTION_MESSAGE(s)	(s + ". " + __FILE__ + ":" + std::to_string(__LINE__)) //++ TODO std::source_location

#ifdef _WIN32
#define FIBO_CP_UTF8	65001 // CP_UTF8
#define F_EXCEPTION_MESSAGE_WINDOWS(s)	(s + ". Error: " + std::to_string(::GetLastError()) + __FILE__ + ":" + std::to_string(__LINE__)) //++ TODO std::source_location
#else
#define FIBO_CP_UTF8	65001 //++ TODO define for Linux
//++ TODO
#endif // _WIN32

#endif