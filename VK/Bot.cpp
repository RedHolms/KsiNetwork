#include "Bot.h"

#include <iostream>

namespace KsiNetwork {
   namespace VK {

      Bot::Bot(std::string accessToken, uint32_t groupId, ApiVersion apiv)
         : m_api(accessToken, apiv), m_lp(m_api, groupId), m_log("VK::Bot"), m_exitCode(0)
      {}

      void Bot::Start() {
         m_th = std::thread([this]() {
            for (size_t i = 0; i < m_layers.size(); i++)
               if (!m_layers[i]->onStart(*this)) {
                  m_log.Warn("Layer #%i failed to load", i);
                  m_layers.erase(m_layers.begin() + i);
               }

            m_active = true;
            m_exitCode = 0;
            m_log.Info("Bot started");
            while (m_active) {
               try {
                  _Process();
               }
               catch (std::exception& e) {
                  m_log.Error("Unhandled exception catched in _Process: %s", e.what());
                  continue;
               }
            }

            for (auto& l : m_layers)
               l->onExit(*this);
         });
      }

      void Bot::_Process() {
         m_lp.Update();
         for (auto& event : m_lp.GetUpdates()) {
            _DispatchEvent(event["type"], event["object"]);
         }
      }

      void Bot::_DispatchEvent(std::string eventType, nlohmann::json eventObject) {
#define _RaiseEvent(vkname, fn) \
               if (eventType == #vkname) { \
                  for (size_t i = 0; i < m_layers.size(); i++) { \
                     try { \
                        m_layers[i]->fn(*this, eventObject); \
                     } \
                     catch(std::exception& e) { \
                        _HandleException(e, i); \
                     } \
                  } \
               }
         // i'm sorry about that but it was the easiest way
               _RaiseEvent(audio_new,                          onAudioNewEvent)
         else  _RaiseEvent(board_post_delete,                  onBoardPostDeleteEvent)
         else  _RaiseEvent(board_post_edit,                    onBoardPostEditEvent)
         else  _RaiseEvent(board_post_new,                     onBoardPostNewEvent)
         else  _RaiseEvent(board_post_restore,                 onBoardPostRestoreEvent)
         else  _RaiseEvent(message_event,                      onCallbackButtonPressEvent)
         else  _RaiseEvent(user_block,                         onFollowerBanEvent)
         else  _RaiseEvent(group_join,                         onFollowerJoinEvent)
         else  _RaiseEvent(group_leave,                        onFollowerLeaveEvent)
         else  _RaiseEvent(user_unblock,                       onFollowerUnbanEvent)
         else  _RaiseEvent(group_officers_edit,                onGroupOfficersListEditEvent)
         else  _RaiseEvent(group_change_photo,                 onGroupPhotoChangeEvent)
         else  _RaiseEvent(group_change_settings,              onGroupSettingsChangeEvent)
         else  _RaiseEvent(market_comment_delete,              onMarketCommentDeleteEvent)
         else  _RaiseEvent(market_comment_edit,                onMarketCommentEditEvent)
         else  _RaiseEvent(market_comment_new,                 onMarketCommentNewEvent)
         else  _RaiseEvent(market_comment_restore,             onMarketCommentRestoreEvent)
         else  _RaiseEvent(market_order_edit,                  onMarketOrderEditEvent)
         else  _RaiseEvent(market_order_new,                   onMarketOrderNewEvent)
         else  _RaiseEvent(message_allow,                      onMessagesAllowEvent)
         else  _RaiseEvent(message_deny,                       onMessagesDenyEvent)
         else  _RaiseEvent(message_edit,                       onMessageEditEvent)
         else  _RaiseEvent(message_new,                        onMessageNewEvent)
         else  _RaiseEvent(message_reply,                      onMessageReplyEvent)
         else  _RaiseEvent(message_typing_state,               onMessageTypingStateChangeEvent)
         else  _RaiseEvent(photo_comment_delete,               onPhotoCommentDeleteEvent)
         else  _RaiseEvent(photo_comment_edit,                 onPhotoCommentEditEvent)
         else  _RaiseEvent(photo_comment_new,                  onPhotoCommentNewEvent)
         else  _RaiseEvent(photo_comment_restore,              onPhotoCommentRestoreEvent)
         else  _RaiseEvent(photo_new,                          onPhotoNewEvent)
         else  _RaiseEvent(poll_vote_new,                      onPollNewVoteEvent)
         else  _RaiseEvent(donut_money_withdraw_error,         onVKDonutMoneyWithdrawErrorEvent)
         else  _RaiseEvent(donut_money_withdraw,               onVKDonutMoneyWithdrawSuccessEvent)
         else  _RaiseEvent(donut_subscription_cancelled,       onVKDonutSubscriptionCancelEvent)
         else  _RaiseEvent(donut_subscription_expired,         onVKDonutSubscriptionExpireEvent)
         else  _RaiseEvent(donut_subscription_create,          onVKDonutSubscriptionNewEvent)
         else  _RaiseEvent(donut_subscription_price_changed,   onVKDonutSubscriptionPriceChangeEvent)
         else  _RaiseEvent(donut_subscription_prolonged,       onVKDonutSubscriptionProlongEvent)
         else  _RaiseEvent(app_payload,                        onVKMiniAppsPayloadEvent)
         else  _RaiseEvent(vkpay_transaction,                  onVKPayTransactionEvent)
         else  _RaiseEvent(video_comment_delete,               onVideoCommentDeleteEvent)
         else  _RaiseEvent(video_comment_edit,                 onVideoCommentEditEvent)
         else  _RaiseEvent(video_comment_new,                  onVideoCommentNewEvent)
         else  _RaiseEvent(video_comment_restore,              onVideoCommentRestoreEvent)
         else  _RaiseEvent(video_new,                          onVideoNewEvent)
         else  _RaiseEvent(like_add,                           onLikeAddEvent)
         else  _RaiseEvent(like_remove,                        onLikeRemoveEvent)
         else  _RaiseEvent(wall_reply_delete,                  onWallPostCommentDeleteEvent)
         else  _RaiseEvent(wall_reply_edit,                    onWallPostCommentEditEvent)
         else  _RaiseEvent(wall_reply_new,                     onWallPostCommentNewEvent)
         else  _RaiseEvent(wall_reply_restore,                 onWallPostCommentRestoreEvent)
         else  _RaiseEvent(wall_post_new,                      onWallPostNewEvent)
         else  _RaiseEvent(wall_repost,                        onWallPostRepostEvent)
         else {
            m_log.Warn("Event with unknown type (%s): %s", eventType, eventObject.dump());
         }
#undef _RaiseEvent
      }

      void Bot::_HandleException(std::exception& e, size_t layerIdx) {
         m_log.Error("Unhandled exception in layer #%i: %s", layerIdx, e.what());
      }

   }
}