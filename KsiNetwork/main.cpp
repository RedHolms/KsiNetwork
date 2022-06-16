#include <Core/core.h>

#include <experimental/filesystem>

#define STRING_UTILS
#include <Core/Utils.h>

#include <VK/Bot.h>

using namespace KsiNetwork;

static Logger g_Log("<main>");

class MainLayer : public VK::Layer {
public:
   MainLayer() : m_log("VK Layer"), m_api("", {0}) {}

private:
   Logger m_log;
   VK::VkApi m_api;

public:
   bool onStart(VK::Bot& bot) {
      m_api = bot.GetApi();
      return true;
   }

   void onMessageNewEvent(VK::Bot& bot, nlohmann::json& eventObject) {
      std::string text = eventObject["message"]["text"].get<std::string>();

      if (text == "exit") ExitCommand(bot, eventObject);
      else if (starts_with(text, "id")) IdCommand(eventObject);
      else UnknownCommand(eventObject);
   }

private:
   void SafeSendMessage(uint32_t peerId, std::string messageText, uint32_t randomId = 0) {
      auto response = m_api.CallMethod("messages.send", {
         { "peer_id", std::to_string(peerId) },
         { "random_id", std::to_string(randomId) },
         { "message", messageText }
      });
      if (response.contains("error")) {
         m_log.Error("Can't send message: %s", response.dump().c_str());
      }
   }

   void Answer(nlohmann::json& eventObject, std::string messageText) {
      SafeSendMessage(eventObject["message"]["from_id"], messageText);
   }

   void ExitCommand(VK::Bot& bot, nlohmann::json& eventObject) {
      Answer(eventObject, "End iteration and exit");
      bot.Exit();
   }

   void IdCommand(nlohmann::json& eventObject) {
      Answer(eventObject, std::to_string(eventObject["message"]["from_id"].get<uint32_t>()));
   }

   void UnknownCommand(nlohmann::json& eventObject) {
      // get user name and surname
      auto response = m_api.CallMethod("users.get", { { "user_ids", std::to_string(eventObject["message"]["from_id"].get<uint32_t>()) } });
      if (response.contains("error")) {
         Answer(eventObject, "error");
         m_log.Error("Can't get user info: %s", response.dump().c_str());
         return;
      }

      Answer(eventObject, response["response"][0]["first_name"].get<std::string>() + " " + response["response"][0]["last_name"].get<std::string>() + ": " + eventObject["message"]["text"].get<std::string>());
   }
};

int main() {
   std::string token = "24c816131b2b42a66efac2cd0920b77358cb107329d6da30d5b750647d26f0a9c92ff54e90e5e29125b67";
   uint32_t groupId = 204308254;

   // setup logger
   Core::MainLogger& log = Core::MainLogger::Get();
   log.AddOutputFile(stdout);
   log.AddOutputFile("KsiNetwork.log");

   g_Log.Debug(L"Working directory: %s", std::experimental::filesystem::current_path().native().c_str());

   // vk initialize
   VK::Bot bot(token, groupId);

   MainLayer* l = new MainLayer;
   bot.AddLayer(l);

   // start everything
   bot.Start();

   // wait for exit
   bot.WaitForExit();

   delete l;
   Core::MainLogger::Destroy();
   return 0;
}