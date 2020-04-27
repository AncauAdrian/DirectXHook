#pragma once
#include "Windows.h"


namespace Pattern
{

	namespace Internal
	{
		bool dataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	}

	DWORD findPattern(DWORD addr, DWORD len, const BYTE* bMask, const char* szMask);
}


