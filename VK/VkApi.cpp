#include "VkApi.h"

#include <iostream>
#include <sstream>

namespace KsiNetwork {
   namespace VK {

      VkApi::VkApi(std::string accessToken, ApiVersion apiVersion)
         : m_accessToken(accessToken), m_log("VK::VkApi")
      {
         m_v = std::to_string(apiVersion.major) + "." + std::to_string(apiVersion.minor);
      }

      nlohmann::json VkApi::CallMethod(std::string method, const ParamsType& _Params) {
         ParamsType params(_Params);
         if (params.find("access_token") == params.end())    params.insert({ "access_token", m_accessToken });
         if (params.find("v") == params.end())               params.insert({ "v", m_v });

         std::string url = "https://api.vk.com/method/" + method;

         std::string rawResponse = Core::Request(url, params);

         nlohmann::json jsonResponse;
         try {
            jsonResponse = nlohmann::json::parse(rawResponse);
         }
         catch (nlohmann::json::parse_error& e) {
            m_log.Fatal(
               "Failed to parse vk api response to json: %s\n"
               "Raw Response: %s",
            e.what(), rawResponse);
            __debugbreak();
         }

         return jsonResponse;
      }

   }
}