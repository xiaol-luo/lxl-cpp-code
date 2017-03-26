#ifdef WIN32

#include "Compat.h"
#include <windows.h>
#include <stdio.h>

namespace Compat
{
	char * GetCurrWorkDir(char *buff, uint32_t buffLen)
	{
		if (nullptr == buff || buffLen <= 0)
			return nullptr;

		GetCurrentDirectory(buffLen, buff);
		return buff;
	}

	int SNPrintf(char *dest, size_t size, const char *format, ...)
	{
		va_list argp;
		va_start(argp, format);
		int ret = _vsnprintf(dest, size, format, argp);
		va_end(argp);
		return ret;
	}

	void SleepMS(uint32_t ms)
	{
		Sleep(ms);
	}
}

#endif