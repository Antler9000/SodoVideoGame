#pragma once
#include <windows.h>
#include <stdexcept>

inline void ThrowIfNull(void* pResult)
{
	if (pResult == nullptr)
	{
		throw std::runtime_error("throw because of null result");
	}
}

inline void ThrowIfFalse(bool bResult)
{
	if (bResult == false)
	{
		throw std::runtime_error("throw because of false result");
	}
}

inline void ThrowIfFailed(HRESULT hResult)
{
	if (FAILED(hResult))
	{
		throw std::runtime_error("throw because of failed result");
	}
}