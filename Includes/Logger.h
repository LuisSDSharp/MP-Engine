#ifndef LOGGER_H_
#define LOGGER_H_

#include "Platforms.h"

namespace Log
{
	void Info ( const char*, ... );
	void Warn ( const char*, ... );
	void Error ( const char*, ... );
	void Assert ( const bool, const char*, ... );
}

#if PLATFORM_ANDROID
#include <android/log.h>
#include <stdio.h>
#define vsprintf_s vsprintf

#define LOG_TAG "MPEngine"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#define LOG_ASSERT(_cond, ...) if(!_cond) ((void)__android_log_assert("conditional", LOG_TAG, __VA_ARGS__))
#elif PLATFORM_WINDOWS
#include <Windows.h>
#include <string>
#include <assert.h>

inline void LOG(const char* pAppend, const char* pMsg)
{
	std::string message(pAppend);
	message.append(pMsg);
	message.append("\n");
	OutputDebugStringA(message.c_str());
}

inline void LOGI(const char* pMsg)
{
	LOG("INFO: ", pMsg);
}

inline void LOGW(const char* pMsg)
{
	LOG("WARN: ", pMsg);
}

inline void LOGE ( const char* pMsg )
{
	LOG ( "ERROR: ", pMsg );
}

inline void LOG_ASSERT ( const bool condition, const char* pMsg )
{
	assert ( condition && pMsg );
}
#endif

#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_FUNCTION_NOT_IMPLEMENTED Log::Warn("Not Implemented: %s in %s", __FUNCTION__, __FILENAME__);

#endif