#ifndef PROTOCOL_H
#define PROTOCOL_H

// json strings
#define JSON_command                   "command"
#define JSON_player                    "player"

// JSON_command
#define JSON_handshake                 "handshake"       // server  <->  client // ok
#define JSON_chat                      "chat"            // server  <->  client // ok
#define JSON_game_start                "game_start"      // server   ->  client // ok
#define JSON_announce                  "announce"        // server  <->  client // ok
#define JSON_exchange                  "exchange"        // server  <->  client
#define JSON_player_turn               "player_turn"     // server   ->  client // ok
#define JSON_play_cards                "play_cards"      // server  <->  client
#define JSON_check                     "check"           // server  <->  client // ok
#define JSON_end_turn                  "end_turn"        // server   ->  client // ok
#define JSON_end_game                  "end_game"        // server   ->  client // ok
#define JSON_error                     "error"           // server  <->  client // maybe...

// JSON_handshake
#define JSON_password                  "password"
#define JSON_welcome                   "welcome"
#define JSON_p_north                   "player_north"
#define JSON_p_east                    "player_east"
#define JSON_p_south                   "player_south"
#define JSON_p_west                    "player_west"

// JSON_chat
#define JSON_text                      "text"

// JSON_game_start
#define JSON_hand                      "hand"

// JSON_player_turn

// JSON_announce
#define JSON_unknown                   "unknown"
#define JSON_none                      "none"
#define JSON_tichu                     "tichu"
#define JSON_grand_tichu               "grand tichu"
#define JSON_artichette                "artichette"       // non fede
#define JSON_announce_error            "invalide announce"
#define JSON_allready_announce_error   "allready announce"
#define JSON_announce_fail             "rage announce"

// JSON_exchange
#define JSON_none                      "none"
#define JSON_ok                        "ok"
#define JSON_give_right                "give_right"
#define JSON_give_left                 "give_left"
#define JSON_give_friend               "give_friend"
#define JSON_receive_right             "receive_right"
#define JSON_receive_left              "receive_left"
#define JSON_receive_front             "receive_front"
#define JSON_exchange_error            "exchange_error"

// JSON_play_cards
#define JSON_visible                   "visible"
#define JSON_upper_hand                "upper_hand"
#define JSON_play_error                "play_error"

// JSON_check

// JSON_end_turn

// blind board status
#define JSON_boars_status              "boars_status"
#define JSON_deal_size                 "deal_size"
#define JSON_discard                   "discard"
#define JSON_last_played               "last_played"
#define JSON_leading_player            "leading_player"
#define JSON_won                       "won" // per player
#define JSON_hand                      "hand" // per player
#define JSON_lead                      "lead" // per player
// visible by player
// TBD

// JSON_end_game
#define JSON_vertical_team             "vertical_team"
#define JSON_horizontal_team           "horizontal_team"
#define JSON_winner                    "winner"
#define JSON_ex_equo                   "ex_equo"

// JSON_error
#define JSON_no_text                   "no text"
#define JSON_wrong_name                "wrong name"
#define JSON_wrong_password            "wrong password"
#define JSON_no_name                   "no name"
#define JSON_error_field               "field error"

#endif // PROTOCOL_H
