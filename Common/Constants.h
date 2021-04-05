#pragma once

#include <stdint.h>

constexpr uint32_t MAX_USER = 1500;

constexpr uint32_t SOCKET_BUFF_SIZE = (1024 * 32);
constexpr uint32_t MAX_PACKET_SIZE  = (1024 * 16);

constexpr uint8_t MAX_ID_SIZE          = 20;
constexpr uint8_t SLOT_MAX             = 14;
constexpr uint8_t HAVE_MAX             = 28;
constexpr uint8_t WAREHOUSE_MAX        = 192;
constexpr uint8_t MAX_PARTY_MEMBERS    = 8;
constexpr uint8_t MAX_QUEST            = 100;
constexpr uint8_t MAX_SAVED_MAGIC      = 10;
constexpr uint8_t MAX_TYPE3_REPEAT     = 40;
constexpr uint8_t MAX_TYPE4_BUFF       = 56;
constexpr uint8_t MAX_MESSAGE_EVENT    = 10;
constexpr uint8_t MAX_COUPON_ID_LENGTH = 20;
constexpr uint8_t MAX_CURRENT_EVENT    = 40;

// User Socket States
enum SocketState
{
    CONNECTED    = 0x01, // When the user enter login information.
    DISCONNECTED = 0x02, // When the user exits the game.
    GAMESTART    = 0x03  // When the user gets inside the game.
};
