#ifndef PROTOCOL_H
#define PROTOCOL_H

// json strings
#define JSON_command        "command"
#define JSON_player         "player"

// JSON_command
#define JSON_handshake      "handshake"       // server  <->  client
#define JSON_chat           "chat"            // server  <->  client
#define JSON_game_start     "game_start"      // server   ->  client
#define JSON_announce       "announce"        // server  <->  client
#define JSON_exchange       "exchange"        // server  <->  client
#define JSON_player_turn    "player_turn"     // server   ->  client
#define JSON_play_cards     "play_cards"      // server  <->  client
#define JSON_check          "check"           // server  <->  client
#define JSON_end_turn       "end_turn"        // server   ->  client
#define JSON_end_game       "end_game"        // server   ->  client
#define JSON_error          "error"           // server  <->  client

// JSON_handshake
#define JSON_password       "password"
#define JSON_welcome        "welcome"

// JSON_chat
#define JSON_text           "text"

// JSON_game_start
#define JSON_hand           "hand"

// JSON_player_turn

// JSON_announce
#define JSON_none           "none"
#define JSON_tichu          "tichu"
#define JSON_grand_tichu    "grand tichu"
#define JSON_artichette     "artichette"       // non fede
#define JSON_announce_error "invalide announce"
#define JSON_announce_fail  "rage announce"

// JSON_exchange
#define JSON_none           "none"
#define JSON_give_right     "give_right"
#define JSON_give_left      "give_left"
#define JSON_give_friend    "give_friend"
#define JSON_receive_right  "receive_right"
#define JSON_receive_left   "receive_left"
#define JSON_receive_front  "receive_front"

// JSON_play_cards
#define JSON_visible        "visible"
#define JSON_upper_hand     "upper_hand"

// JSON_check

// JSON_end_turn
#define JSON_boars_status   "boars_status"
#define JSON_in_game        "in_game"
#define JSON_won            "won"
#define JSON_last_played    "last_played"
// TBD

// JSON_end_game

// JSON_error
#define JSON_no_text        "no text"
#define JSON_wrong_name     "wrong name"
#define JSON_wrong_password "wrong password"
#define JSON_no_name        "no name"
#define JSON_error_field    "field error"

#endif // PROTOCOL_H
