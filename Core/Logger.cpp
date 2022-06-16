#include "Logger.h"

#include <chrono>
#include <thread>

static KsiNetwork::Logger g_log("Core");
static KsiNetwork::Core::MainLogger* g_MainLoggerInstance = nullptr;

namespace KsiNetwork {
   namespace Core {

      MainLogger& MainLogger::Get() {
         if (!g_MainLoggerInstance)
            g_MainLoggerInstance = new MainLogger;
         return *g_MainLoggerInstance;
      }

      void MainLogger::Destroy() {
         if (!g_MainLoggerInstance) return;
         delete g_MainLoggerInstance;
         g_MainLoggerInstance = nullptr;
      }

      MainLogger::MainLogger()
         : m_logLevel(DEBUG_SWITCH(LogLevel::Debug, LogLevel::Info)) {}

      MainLogger::~MainLogger() {
         for (auto& f : m_outFiles)
            if (f.close) fclose(f.f);
      }

      void MainLogger::PrintMessage(const LogMessage& msg) {
         std::thread([this](LogMessage msg) {

            wchar_t* outBuff = _FormatMessage(msg);
            for (auto& f : m_outFiles)
               fwprintf(f.f, L"%s\n", outBuff), fflush(f.f);
            delete[] outBuff;
            delete[] msg.buff;

         }, msg).detach();
      }

      /* Example:
            [12:00:00.000000] (INFO/KsiNetwork) Hello, World!
      */
      wchar_t* MainLogger::_FormatMessage(const LogMessage& msg) {
         wchar_t* result = new wchar_t[1024];

         struct tm serializedTime;
         time_t seconds = msg.time / 1000000; // make seconds from microseconds(1 microsecond = 0.000001 second)
         localtime_s(&serializedTime, &seconds);
         char* timeBuff = new char[32];
         strftime(timeBuff, 32, "%T", &serializedTime);

         const char* strLogLevel = nullptr;
         switch (msg.logLevel) {
            case LogLevel::Debug: strLogLevel = "DEBUG"; break;
            case LogLevel::Info: strLogLevel = "INFO"; break;
            case LogLevel::Warn: strLogLevel = "WARN"; break;
            case LogLevel::Error: strLogLevel = "ERROR"; break;
            case LogLevel::Fatal: strLogLevel = "FATAL"; break;
            default: strLogLevel = "UNKNOWN";
         }

         swprintf(result, 1024,
            msg.wide ? L"[%hs.%.6llu] (%hs/%hs) %s" : L"[%hs.%.6llu] (%hs/%hs) %hs",
            timeBuff, msg.time % 1000000, strLogLevel, msg.sender, msg.buff
         );
         delete[] timeBuff;
         return result;
      }
   }
}