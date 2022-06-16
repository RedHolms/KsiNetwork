#pragma once

#include <Core/core.h>

#include <nlohmann/json.hpp>


namespace KsiNetwork {
   namespace VK {

      class Bot;

      AbstractClass
      class Layer {
      public:
         /* system events */
         virtual bool onStart(Bot&) { return true; } // false = Layer not loaded successfuly: print warning and remove the layer
         virtual void onExit(Bot&) {}

         /* vk events */
         virtual void onAudioNewEvent                       (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onBoardPostDeleteEvent                (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onBoardPostEditEvent                  (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onBoardPostNewEvent                   (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onBoardPostRestoreEvent               (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onCallbackButtonPressEvent            (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onFollowerBanEvent                    (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onFollowerJoinEvent                   (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onFollowerLeaveEvent                  (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onFollowerUnbanEvent                  (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onGroupOfficersListEditEvent          (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onGroupPhotoChangeEvent               (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onGroupSettingsChangeEvent            (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMarketCommentDeleteEvent            (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMarketCommentEditEvent              (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMarketCommentNewEvent               (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMarketCommentRestoreEvent           (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMarketOrderEditEvent                (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMarketOrderNewEvent                 (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMessagesAllowEvent                  (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMessagesDenyEvent                   (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMessageEditEvent                    (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMessageNewEvent                     (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMessageReplyEvent                   (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onMessageTypingStateChangeEvent       (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onPhotoCommentDeleteEvent             (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onPhotoCommentEditEvent               (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onPhotoCommentNewEvent                (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onPhotoCommentRestoreEvent            (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onPhotoNewEvent                       (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onPollNewVoteEvent                    (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutMoneyWithdrawErrorEvent      (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutMoneyWithdrawSuccessEvent    (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutSubscriptionCancelEvent      (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutSubscriptionExpireEvent      (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutSubscriptionNewEvent         (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutSubscriptionPriceChangeEvent (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKDonutSubscriptionProlongEvent     (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKMiniAppsPayloadEvent              (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVKPayTransactionEvent               (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVideoCommentDeleteEvent             (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVideoCommentEditEvent               (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVideoCommentNewEvent                (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVideoCommentRestoreEvent            (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onVideoNewEvent                       (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onLikeAddEvent                        (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onLikeRemoveEvent                     (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onWallPostCommentDeleteEvent          (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onWallPostCommentEditEvent            (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onWallPostCommentNewEvent             (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onWallPostCommentRestoreEvent         (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onWallPostNewEvent                    (Bot& bot, nlohmann::json& eventObject) {}
         virtual void onWallPostRepostEvent                 (Bot& bot, nlohmann::json& eventObject) {}

         /*
         this class is abstract so make sure we can't create instance of it
         but we need to have access to constructor from child classes
         */
      protected:
         Layer() {}
         ~Layer() {}
         Layer(const Layer&) {}
         Layer& operator=(const Layer&) {}
      };

   }
}
