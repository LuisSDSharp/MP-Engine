#include "../Includes/Logger.h"

namespace Log
{
	void Info ( const char* msg, ... )
	{
		va_list args;
		va_start ( args, msg );
		char szBuf [1024];
		vsprintf_s ( szBuf, msg, args );

		LOGI ( szBuf );
	}

	void Warn ( const char* msg, ... )
	{
		va_list args;
		va_start ( args, msg );
		char szBuf [1024];
		vsprintf_s ( szBuf, msg, args );

		LOGW ( szBuf );
	}

	void Error ( const char* msg, ... )
	{
		va_list args;
		va_start ( args, msg );
		char szBuf [1024];
		vsprintf_s ( szBuf, msg, args );

		LOGE ( szBuf );
	}

	void Assert ( const bool condition, const char * msg, ... )
	{
		va_list args;
		va_start ( args, msg );
		char szBuf [1024];
		vsprintf_s ( szBuf, msg, args );

		LOG_ASSERT ( condition, szBuf );
	}
}
