#pragma once

//
// This is a define only file so we don't need a namespace
//

#if defined(_WIN32)
#define AETH_WINDOWS

#elif defined(__APPLE__)
#define AETH_APPLE
#ifdef TARGET_OS_IPHONE // This is a XCODE def so unlikely Aether will ever see this but we need to be prepared
#define AETH_APPLE_MOBILE
#else
#define AETH_APPLE_UNKNOWN
#endif

#elif defined(__unix__)
#define AETH_UNIX
#if defined(__ANDROID__)
#define AETH_ANDROID
#elif defined(__linux__)
#define AETH_LINUX
#elif defined(__FreeBSD__)
#define	AETH_FREEBSD
#elif defined(__CYGWIN__)
#define AETH_CYGWIN
#endif

#endif // _WIN32 || __APPLE__ || __unix__

#if defined(_x86)
#define AETH_x86

#elif defined(_x64)
#define AETH_x64

#endif // x86 || x64

#if defined(_DEBUG)
#define AETH_DEBUG

#endif // _DEBUG
