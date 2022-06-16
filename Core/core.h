#pragma once

/* normalize debug defines */
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#elif defined(DEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif

/* to check debug like this: #if DEBUG_BUILD */
#if defined(DEBUG)
#define DEBUG_BUILD 1
#else
#define DEBUG_BUILD 0
#endif

/* DEBUG_SWITCH(First, Second): If debug enabled => First, else => Second */
#if defined(DEBUG)
#define DEBUG_SWITCH(on, off) on
#else
#define DEBUG_SWITCH(on, off) off
#endif

#define KSINETWORK_START namespace KsiNetwork {
#define KSINETWORK_END }

/* i'm using c++14, so some content is experimental and "deprecated" */
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

/* some marks for classes */
#define AbstractClass
#define Singleton
#define Wrapper(cls)

/* we needs more secure ;) */
#if defined(__STDC_WANT_SECURE_LIB__)
#undef __STDC_WANT_SECURE_LIB__
#endif
#define __STDC_WANT_SECURE_LIB__ 1

/* some of core includes */
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>

/* things for interfaces */
#if defined(_MSC_BUILD)
#define _INTERFACE __interface
#define _INTERFACE_METHOD(def, name, ...) public: virtual def name(__VA_ARGS__) = 0
#else
#define _INTERFACE struct
#define _INTERFACE_METHOD(def, name, ...) virtual def name(__VA_ARGS__) = 0
#endif