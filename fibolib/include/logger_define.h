#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL	SPDLOG_LEVEL_DEBUG
#define SPDLOG_DEBUG_ON 1
#endif // _DEBUG
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT 1
#include "spdlog/spdlog.h"