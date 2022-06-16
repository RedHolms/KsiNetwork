#pragma once

#include <Core/core.h>

#include <vector>
#include <thread>

#include <nlohmann/json.hpp>

#include <Core/Logger.h>

#include "LongPoll.h"
#include "Layer.h"
#include "VkApi.h"

namespace KsiNetwork {
   namespace VK {

      class Bot {
      public:
         Bot(std::string accessToken, uint32_t groupId, ApiVersion apiv = { 5, 131 });

      private:
         VkApi m_api;
         LongPoll m_lp;
         std::vector<Layer*> m_layers;
         std::thread m_th;

         bool m_active = false;
         int m_exitCode;

         Logger m_log;

      public:
         void Start();
         void Exit() { m_active = false; }

         void WaitForExit() { if (m_th.joinable()) m_th.join(); }

         VkApi& GetApi() { return m_api; }
         LongPoll& GetLP() { return m_lp; }

         void AddLayer(Layer* l) { m_layers.push_back(l); }
         bool RemoveLayer(Layer* l) {
            for (auto it = m_layers.begin(); it != m_layers.end(); it++)
               if (*it == l) { m_layers.erase(it); return true; }
            return false;
         }

      private:
         void _Process();
         void _DispatchEvent(std::string eventType, nlohmann::json eventObject);
         void _HandleException(std::exception& e, size_t layerIdx);
      };

   }
}