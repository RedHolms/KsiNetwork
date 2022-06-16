#pragma once
#include "core.h"

template <typename _To, typename _From>
_To constexpr unconditional_cast(_From _Value) { return *reinterpret_cast<_To*>(&_Value); }

template <typename _A, typename _B>
struct compare_types {
   static constexpr bool check(_B*) { return true; }
   static constexpr bool check(...) { return false; }
   enum : bool { equal = check(static_cast<_A*>(nullptr)) };
};

char* AllocateAndFormatV(const char* fmt, va_list args);
wchar_t* AllocateAndFormatV(const wchar_t* fmt, va_list args);

inline char* AllocateAndFormat(const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   char* buff = AllocateAndFormatV(fmt, args);
   va_end(args);
   return buff;
}
inline wchar_t* AllocateAndFormat(const wchar_t* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   wchar_t* buff = AllocateAndFormatV(fmt, args);
   va_end(args);
   return buff;
}

#if defined(STRING_UTILS) || defined(ALL_UTILS)

#include <string>

template <typename _CharT>
bool constexpr starts_with(std::basic_string<_CharT> str, const _CharT* subStr) { return str.substr(0, strlen(subStr)) == subStr; }

template <typename _CharT>
bool constexpr starts_with(std::basic_string<_CharT> str, std::basic_string<_CharT> subStr) { return str.substr(0, subStr.size()) == subStr; }

#endif