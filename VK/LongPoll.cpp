#include "LongPoll.h"

#include <iostream>

namespace KsiNetwork {
   namespace VK {

      LongPoll::LongPoll(const VkApi& api, uint32_t groupId)
         : m_api(api), m_groupId(groupId), m_log("VK::LongPoll")
      {
         UpdateLpInfo();
      }

      void LongPoll::UpdateLpInfo() {
         auto response = m_api.CallMethod("groups.getLongPollServer", {
            { "group_id", std::to_string(m_groupId) }
            });
         assert(!response.contains("error"));

         m_server = response["response"]["server"];
         m_key = response["response"]["key"];
         m_ts = response["response"]["ts"];
      }

      void LongPoll::Update() {
         std::string rawResponse = Core::Request(m_server, {
            { "act", "a_check" },
            { "key", m_key },
            { "ts", m_ts },
            { "wait", "25" }
            });

         nlohmann::json response;
         try {
            response = nlohmann::json::parse(rawResponse);
         }
         catch (nlohmann::json::parse_error& e) {
            m_log.Fatal(
               "Failed to parse long poll response to json: %s\n"
               "Raw Response: %s",
               e.what(), rawResponse);
            __debugbreak();
         }

         if (response.contains("failed")) {
            switch (response["failed"].get<uint32_t>()) {
            case 1:
               m_ts = response["ts"];
               return Update();
            case 2:
            case 3:
               UpdateLpInfo();
               return Update();
            default:
               __debugbreak();
               break;
            }
         }

         m_updates = response["updates"];
         m_ts = response["ts"];
      }

   }
}