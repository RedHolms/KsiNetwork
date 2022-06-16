#pragma once
#include <Core/core.h>

#include <string>
#include <map>

#include <nlohmann/json.hpp>

#include <Core/Requests.h>
#include <Core/Logger.h>

namespace KsiNetwork {
   namespace VK {

      using ParamsType = Core::HttpParams;

      struct ApiVersion {
         uint16_t major;
         uint16_t minor;
      };

      class VkApi {
      public:
         VkApi(std::string accessToken, ApiVersion apiVersion);

      private:
         std::string m_accessToken;
         std::string m_v;

         Logger m_log;

      public:
         nlohmann::json CallMethod(std::string method, const ParamsType& params);
      };

   }
}