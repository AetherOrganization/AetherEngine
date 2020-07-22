#pragma once

#ifdef _LINUX
#define LIB_API
#elif _WINDOWS
#ifdef AETHERENGINE_EXPORTS
#define LIB_API extern "C" __declspec(dllexport)
#else
#define LIB_API extern "C" __declspec(dllimport)
#endif
#endif

LIB_API void TestFunctionOnee();
LIB_API void TestFunctionTwo(const char* tmp);