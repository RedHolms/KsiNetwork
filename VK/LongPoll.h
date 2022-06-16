#pragma once

#include <Core/core.h>

#include <nlohmann/json.hpp>

#include "VkApi.h"

namespace KsiNetwork {
   namespace VK {

      class LongPoll {
      public:
         LongPoll(const VkApi& api, uint32_t groupId);

      private:
         VkApi m_api;
         uint32_t m_groupId;
         nlohmann::json m_updates;

         std::string m_server;
         std::string m_key;
         std::string m_ts;

         Logger m_log;

      public:
         void UpdateLpInfo();
         void Update();

         nlohmann::json GetUpdates() { return m_updates; }
      };

   }
}