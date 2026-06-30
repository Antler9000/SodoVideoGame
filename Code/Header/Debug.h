#pragma once
#include <windows.h>
#include <stdexcept>
#include <crtdbg.h>

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
	if (FAILED(hResult) == true)
	{
		throw std::runtime_error("throw because of failed result");
	}
}

//NOTE :	메모리 누수로 이어진 new 할당을 추적하기 위해 오버로딩된 new를 디버깅 과정에서 사용할 수 있도록 함
//			이를 명시적으로 사용해야 하므로, make_unique 대신 unique_ptr<T>(DBG_NEW T(argument))를 사용해야 함
#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
	#define DBG_NEW new
#endif
