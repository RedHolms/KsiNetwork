#define ALL_UTILS
#include "Utils.h"

#include <stdio.h>

char* AllocateAndFormatV(const char* fmt, va_list args) {
   va_list args2;
   va_copy(args2, args);

   int buffSize = vsnprintf(nullptr, 0, fmt, args2) + 1;
   va_end(args2);

   char* buff = new char[buffSize];

   vsnprintf(buff, buffSize, fmt, args);
   return buff;
}

wchar_t* AllocateAndFormatV(const wchar_t* fmt, va_list args) {
   va_list args2;
   va_copy(args2, args);

   int buffSize = vswprintf(nullptr, 0, fmt, args2) + 1;
   va_end(args2);

   wchar_t* buff = new wchar_t[buffSize];

   vswprintf(buff, buffSize, fmt, args);
   return buff;
}