#include <string.h>

namespace Util
{
	bool IsSuffitMatch(const char *path, const char *suffit)
	{
		if (NULL == path || NULL == suffit)
			return false;

		int pathLen = strlen(path);
		int suffitLen = strlen(suffit);

		if (suffitLen > pathLen)
			return false;

		if (0 != strncmp(path + pathLen - suffitLen, suffit, suffitLen))
			return false;

		return true;
	}
}

