#pragma once
#include "core.h"

#include <vector>
#include <chrono>

#include <stdio.h>

#include "Utils.h"

KSINETWORK_START

enum class LogLevel {
   Debug,
   Info,
   Warn,
   Error,
   Fatal
};

namespace Core {

   struct LogMessage {
      bool wide;
      union {
         char* buff;
         wchar_t* wbuff;
      };
      LogLevel logLevel;
      const char* sender;
      long long time; // microseconds
   };

   struct LogFile {
      FILE* f;
      bool close;
   };

   Singleton
   class MainLogger {
   public:
      static MainLogger& Get();
      static void Destroy();

      MainLogger(const MainLogger&) = delete;
      MainLogger& operator=(const MainLogger&) = delete;
   private:
      MainLogger();
      ~MainLogger();

   private:
      LogLevel m_logLevel;
      std::vector<LogFile> m_outFiles;

   public:
      LogLevel GetLogLevel()                    { return m_logLevel; }
      void     SetLogLevel(LogLevel logLevel)   { m_logLevel = logLevel; }

      template <typename _CharT>
      void Log(const char* moduleName, LogLevel logLevel, const _CharT* fmt, ...) {
         if (logLevel < m_logLevel) return;

         va_list args;
         va_start(args, fmt);
         _CharT* buff = AllocateAndFormatV(fmt, args);
         va_end(args);

         PrintMessage(NewMessage(buff, moduleName, logLevel));
      }

      void PrintMessage(const LogMessage& msg);

      void AddOutputFile(FILE* f, bool close = false) {
         m_outFiles.push_back({ f, close });
      }
      void AddOutputFile(const char* filename, bool close = true) {
         LogFile file{};
         fopen_s(&file.f, filename, "w+");
         file.close = close;
         m_outFiles.push_back(file);
      }

      template <typename _CharT>
      static constexpr LogMessage NewMessage(_CharT* buff, const char* sender, LogLevel logLevel) {
         LogMessage msg{ 0 };
         msg.wide = compare_types<decltype(buff), wchar_t*>::equal;
         msg.buff = reinterpret_cast<char*>(buff);
         msg.logLevel = logLevel;
         msg.sender = sender ? sender : "<unknown>";
         msg.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
         return msg;
      }

   private:
      wchar_t* _FormatMessage(const LogMessage& msg);
   };

}

Wrapper(Core::MainLogger)
class Logger {
public:
   Logger(const char* moduleName) : m_moduleName(moduleName) {}

private:
   const char* m_moduleName;

public:
   template <typename _CharT, typename... _Args>
   void Log(LogLevel logLevel, const _CharT* fmt, _Args... args) { return Core::MainLogger::Get().Log(m_moduleName, logLevel, fmt, args...); }

   template <typename _CharT, typename... _Args>
   void Debug(const _CharT* fmt, _Args... args) { return Log(LogLevel::Debug, fmt, args...); }

   template <typename _CharT, typename... _Args>
   void Info(const _CharT* fmt, _Args... args) { return Log(LogLevel::Info, fmt, args...); }

   template <typename _CharT, typename... _Args>
   void Warn(const _CharT* fmt, _Args... args) { return Log(LogLevel::Warn, fmt, args...); }

   template <typename _CharT, typename... _Args>
   void Error(const _CharT* fmt, _Args... args) { return Log(LogLevel::Error, fmt, args...); }

   template <typename _CharT, typename... _Args>
   void Fatal(const _CharT* fmt, _Args... args) { return Log(LogLevel::Fatal, fmt, args...); }
};

KSINETWORK_END