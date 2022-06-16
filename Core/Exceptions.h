#pragma once
#include "core.h"

#include <exception>
#include <string>

enum class KsiExceptionCode {
   UNKNOWN = 0,
   CURL_FAILED
};

/* base exceptions class */
class KsiNetworkException : public std::exception {
public:
   KsiNetworkException(const char* what, KsiExceptionCode ecode)
      : m_ecode(ecode), std::exception(what) {}

   KsiExceptionCode GetExceptionCode() { return m_ecode; }
  
private:
   KsiExceptionCode m_ecode;
};

class CurlFailedException : public KsiNetworkException {
public:
   CurlFailedException(int curlCode, std::string errorInfo)
      : m_curlCode(curlCode), m_errorInfo(errorInfo), KsiNetworkException("curl_***_perform() failed", KsiExceptionCode::CURL_FAILED) {}

private:
   int m_curlCode;
   std::string m_errorInfo;
};