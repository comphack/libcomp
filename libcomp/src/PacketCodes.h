/**
 * @file libcomp/src/PacketCodes.h
 * @ingroup libcomp
 *
 * @author HACKfrost
 *
 * @brief Contains enums for internal and external packet codes.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2012-2020 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBCOMP_SRC_PACKETCODES_H
#define LIBCOMP_SRC_PACKETCODES_H

// libcomp Includes
#include "EnumUtils.h"

// Standard C++11 Includes
#include <stdint.h>

/**
 * Request packet code from the game client to the lobby server.
 */
enum class ClientToLobbyPacketCode_t : uint16_t {
  /// Login request.
  PACKET_LOGIN = 0x0003,
  /// Authorization request.
  PACKET_AUTH = 0x0005,
  /// Start game request.
  PACKET_START_GAME = 0x0007,
  /// Character list request.
  PACKET_CHARACTER_LIST = 0x0009,
  /// World list request.
  PACKET_WORLD_LIST = 0x000B,
  /// Create character request.
  PACKET_CREATE_CHARACTER = 0x000D,
  /// Delete character request.
  PACKET_DELETE_CHARACTER = 0x000F,
  /// Query purchase ticket request.
  PACKET_QUERY_PURCHASE_TICKET = 0x0011,
  /// Purchase ticket request.
  PACKET_PURCHASE_TICKET = 0x0013,
};

/**
 * Response packet code from the lobby server to the game client.
 */
enum class LobbyToClientPacketCode_t : uint16_t {
  /// Login response.
  PACKET_LOGIN = 0x0004,
  /// Authorization response.
  PACKET_AUTH = 0x0006,
  /// Start game response.
  PACKET_START_GAME = 0x0008,
  /// Character list response.
  PACKET_CHARACTER_LIST = 0x000A,
  /// World list response.
  PACKET_WORLD_LIST = 0x000C,
  /// Create character response.
  PACKET_CREATE_CHARACTER = 0x000E,
  /// Delete character response.
  PACKET_DELETE_CHARACTER = 0x0010,
  /// Query purchase ticket response.
  PACKET_QUERY_PURCHASE_TICKET = 0x0012,
  /// Purchase ticket response.
  PACKET_PURCHASE_TICKET = 0x0014,
};

/**
 * Request packet code from the game client to the channel server.
 */
enum class ClientToChannelPacketCode_t : uint16_t {
  /// Login request.
  PACKET_LOGIN = 0x0000,
  /// Authorization request.
  PACKET_AUTH = 0x0002,
  /// Request for data from the server.
  PACKET_SEND_DATA = 0x0004,
  /// Logout request.
  PACKET_LOGOUT = 0x0005,
  /// Request to populate a zone with game objects and entities.
  PACKET_POPULATE_ZONE = 0x0019,
  /// Request to move an entity or object.
  PACKET_MOVE = 0x001C,
  /// Request to stop a player entity in place for a set amount of time.
  PACKET_PIVOT = 0x0020,
  /// Unused. Request to (re-)sync the client and server times.
  PACKET_SYNC_TIME = 0x0024,
  /// Request to add a message to the chat or process a GM command.
  PACKET_CHAT = 0x0026,
  /// Request to send a chat message to a specific player in the world.
  PACKET_TELL = 0x0027,
  /// Request to activate a player or demon skill.
  PACKET_SKILL_ACTIVATE = 0x0030,
  /// Request to execute a skill that has finished charging.
  PACKET_SKILL_EXECUTE = 0x0031,
  /// Request to execute a skill that has finished charging.
  PACKET_SKILL_CANCEL = 0x0032,
  /// Request to target/retarget a skill being used.
  PACKET_SKILL_TARGET = 0x003D,
  /// Request to lower a character's expertise rank or class.
  PACKET_EXPERTISE_DOWN = 0x0042,
  /// Request to allocate a skill point for a character.
  PACKET_ALLOCATE_SKILL_POINT = 0x0049,
  /// Request to enable or disable an expertise.
  PACKET_TOGGLE_EXPERTISE = 0x004F,
  /// Request for a character to learn a skill.
  PACKET_LEARN_SKILL = 0x0051,
  /// Request for a demon's learned skill set to be updated.
  PACKET_DEMON_SKILL_UPDATE = 0x0054,
  /// Request/check to keep the connection alive.
  PACKET_KEEP_ALIVE = 0x0056,
  /// Request to fix a game object's position.
  PACKET_FIX_OBJECT_POSITION = 0x0058,
  /// Request for the client's character state.
  PACKET_STATE = 0x005A,
  /// Request for the client's active demon state.
  PACKET_DEMON_DATA = 0x005B,
  /// Demon box list request.
  PACKET_DEMON_BOX = 0x005C,
  /// Demon box demon data request.
  PACKET_DEMON_BOX_DATA = 0x005E,
  /// Request for the list of channels connected to the world.
  PACKET_CHANNEL_LIST = 0x0063,
  /// Request to revive the client's character.
  PACKET_REVIVE_CHARACTER = 0x0067,
  /// Request to stop the movement of an entity or object.
  PACKET_STOP_MOVEMENT = 0x006F,
  /// A spot has been triggered by the client.
  PACKET_SPOT_TRIGGERED = 0x0071,
  /// Request for the server's current world time.
  PACKET_WORLD_TIME = 0x0072,
  /// Request for info about a specific item box.
  PACKET_ITEM_BOX = 0x0074,
  /// Request to move an item in an item box.
  PACKET_ITEM_MOVE = 0x0076,
  /// Request to throw away an item from an item box.
  PACKET_ITEM_DROP = 0x0077,
  /// Request to stack or split stacked items in an item box.
  PACKET_ITEM_STACK = 0x0078,
  /// Request for equipment information.
  PACKET_EQUIPMENT_LIST = 0x007B,
  /// Request to start a trade with another player.
  PACKET_TRADE_REQUEST = 0x007D,
  /// Request to accept the trade request from another player.
  PACKET_TRADE_ACCEPT = 0x0080,
  /// Request to add an item to the current trade offer.
  PACKET_TRADE_ADD_ITEM = 0x0082,
  /// Request to lock the current trade for acceptance.
  PACKET_TRADE_LOCK = 0x0085,
  /// Request to finish the current trade.
  PACKET_TRADE_FINISH = 0x0088,
  /// Request to cancel the current trade.
  PACKET_TRADE_CANCEL = 0x008B,
  /// Request to loot an item from a loot box.
  PACKET_LOOT_ITEM = 0x008E,
  /// Request for the current account's total CP.
  PACKET_CASH_BALANCE = 0x0090,
  /// Request for details about an NPC shop.
  PACKET_SHOP_DATA = 0x0092,
  /// Request to buy an item from a shop.
  PACKET_SHOP_BUY = 0x0094,
  /// Request to sell an item to a shop.
  PACKET_SHOP_SELL = 0x0096,
  /// Request to move a demon in a box.
  PACKET_DEMON_BOX_MOVE = 0x009A,
  /// Request to dismiss a demon.
  PACKET_DEMON_DISMISS = 0x009B,
  /// Request to list items in the player account's Post.
  PACKET_POST_LIST = 0x009C,
  /// Request to move an item from the Post into the current character's
  /// inventory.
  PACKET_POST_ITEM = 0x009E,
  /// Request to open/retrieve post gift information.
  PACKET_POST_GIFT = 0x00A0,
  /// Request for data about a hotbar page.
  PACKET_HOTBAR_DATA = 0x00A2,
  /// Request to save a hotbar page.
  PACKET_HOTBAR_SAVE = 0x00A4,
  /// Message that the player has responded to the current event.
  PACKET_EVENT_RESPONSE = 0x00B7,
  /// Request for a list of obtained valuables.
  PACKET_VALUABLE_LIST = 0x00B8,
  /// An object has been clicked on.
  PACKET_OBJECT_INTERACTION = 0x00BA,
  /// Request for the current player's own friend information.
  PACKET_FRIEND_INFO = 0x00BD,
  /// Request to ask a player to be added to the player's friend list.
  PACKET_FRIEND_REQUEST = 0x00C0,
  /// Request to add the player and the requestor to both friend lists.
  PACKET_FRIEND_ADD = 0x00C3,
  /// Request to remove the player and the target from both friend lists.
  PACKET_FRIEND_REMOVE = 0x00C4,
  /// Request to update friend list specific data for the player.
  PACKET_FRIEND_DATA = 0x00C6,
  /// Request to invite a player to join the player's party.
  PACKET_PARTY_INVITE = 0x00D2,
  /// Request to join a party from which an invite was received.
  PACKET_PARTY_JOIN = 0x00D5,
  /// Request to cancel a party invite.
  PACKET_PARTY_CANCEL = 0x00D8,
  /// Request to leave the player's current party.
  PACKET_PARTY_LEAVE = 0x00DA,
  /// Request to disband the current party.
  PACKET_PARTY_DISBAND = 0x00DD,
  /// Request to update the current party's designated leader.
  PACKET_PARTY_LEADER_UPDATE = 0x00E0,
  /// Request to update the current party's drop rule setting.
  PACKET_PARTY_DROP_RULE = 0x00E3,
  /// Response signifying whether a party member update was received correctly.
  PACKET_PARTY_MEMBER_UPDATE = 0x00E6,
  /// Request to kick a player from the current party.
  PACKET_PARTY_KICK = 0x00EB,
  /// Request to fuse two demons into a new demon.
  PACKET_DEMON_FUSION = 0x00EF,
  /// Request for information about the demon in a demon egg.
  PACKET_LOOT_DEMON_EGG_DATA = 0x00F1,
  /// Request to retrieve the server time.
  PACKET_SYNC = 0x00F3,
  /// Request to repair an item at a shop.
  PACKET_SHOP_REPAIR = 0x00F6,
  /// Request to rotate an entity or object.
  PACKET_ROTATE = 0x00F8,
  /// Request for the list of items inside a boss loot box.
  PACKET_LOOT_BOSS_BOX = 0x00FC,
  /// Request to receive union information.
  PACKET_UNION_FLAG = 0x0100,
  /// Request to list the client account's item depositories.
  PACKET_ITEM_DEPO_LIST = 0x0102,
  /// Request to rent a client account item or demon depository.
  PACKET_DEPO_RENT = 0x0104,
  /// Request for the list of items inside a treasure loot box.
  PACKET_LOOT_TREASURE_BOX = 0x0107,
  /// Request for the player's active quest list.
  PACKET_QUEST_ACTIVE_LIST = 0x010B,
  /// Request for the player's completed quest list.
  PACKET_QUEST_COMPLETED_LIST = 0x010C,
  /// Request to open a market at a bazaar.
  PACKET_BAZAAR_MARKET_OPEN = 0x0113,
  /// Request to close the player's current open bazaar market.
  PACKET_BAZAAR_MARKET_CLOSE = 0x0115,
  /// Request for details about a specific bazaar market.
  PACKET_BAZAAR_MARKET_INFO = 0x0117,
  /// Request to add an item to the player's bazaar market.
  PACKET_BAZAAR_ITEM_ADD = 0x0119,
  /// Request to drop an item from the player's bazaar market.
  PACKET_BAZAAR_ITEM_DROP = 0x011B,
  /// Request to update an item in the player's bazaar market.
  PACKET_BAZAAR_ITEM_UPDATE = 0x011D,
  /// Request to buy an item from a bazaar market.
  PACKET_BAZAAR_ITEM_BUY = 0x011F,
  /// Request to take sales macca from the player's bazaar market.
  PACKET_BAZAAR_MARKET_SALES = 0x0122,
  /// Unknown. Request to exchange one item for another.
  PACKET_BAZAAR_ITEM_EXCHANGE = 0x0124,
  /// Request for the player's current search entries of a specified type.
  PACKET_SEARCH_ENTRY_SELF = 0x0128,
  /// Request to list search entries of a specified type that can be filtered.
  PACKET_SEARCH_LIST = 0x012A,
  /// Request for detailed information about a specific search entry.
  PACKET_SEARCH_ENTRY_DATA = 0x012C,
  /// Request to register a new search entry.
  PACKET_SEARCH_ENTRY_REGISTER = 0x012E,
  /// Request to update an existing search entry.
  PACKET_SEARCH_ENTRY_UPDATE = 0x0130,
  /// Request to remove an existing search entry.
  PACKET_SEARCH_ENTRY_REMOVE = 0x0132,
  /// Request to perform an action based on a search entry application.
  PACKET_SEARCH_APPLICATION_REPLY = 0x0135,
  /// Request to disband the player character's clan.
  PACKET_CLAN_DISBAND = 0x0138,
  /// Request to invite a character to the player character's clan.
  PACKET_CLAN_INVITE = 0x013B,
  /// Request to accept an invite to join a clan.
  PACKET_CLAN_JOIN = 0x013E,
  /// Request to reject an invite to join a clan.
  PACKET_CLAN_CANCEL = 0x0140,
  /// Request to kick a character from the player character's clan.
  PACKET_CLAN_KICK = 0x0142,
  /// Request to update a clan's master member.
  PACKET_CLAN_MASTER_UPDATE = 0x0145,
  /// Request to toggle a player's sub-master member type.
  PACKET_CLAN_SUB_MASTER_UPDATE = 0x0148,
  /// Request to leave the player character's clan.
  PACKET_CLAN_LEAVE = 0x014B,
  /// Request to send a chat message to the current clan's channel.
  PACKET_CLAN_CHAT = 0x014E,
  /// Request for the current player character's clan information.
  PACKET_CLAN_INFO = 0x0150,
  /// Request for member information from the current player character's clan.
  PACKET_CLAN_LIST = 0x0152,
  /// Request to update clan member information about the current player.
  PACKET_CLAN_DATA = 0x0154,
  /// Request to form a new clan from a clan formation item.
  PACKET_CLAN_FORM = 0x0156,
  /// Request for an item price estimate based off several criteria.
  PACKET_ITEM_PRICE = 0x0157,
  /// Request for the current zone's bazaar cost and duration.
  PACKET_BAZAAR_STATE = 0x015F,
  /// Request to set the player's bazaar clerk NPC.
  PACKET_BAZAAR_CLERK_SET = 0x0161,
  /// Request to get a suggested sales price for a bazaar item.
  PACKET_BAZAAR_PRICE = 0x0164,
  /// Request for details about the player's bazaar market.
  PACKET_BAZAAR_MARKET_INFO_SELF = 0x0166,
  /// Request to warp the player to the specified warp point.
  PACKET_WARP = 0x016A,
  /// Request to execute a skill immediately, bypassing normal activation.
  PACKET_SKILL_EXECUTE_INSTANT = 0x016B,
  /// Request to sync a client entity's basic information.
  PACKET_SYNC_CHARACTER = 0x017E,
  /// Request to update the current partner demon's AI attack settings.
  PACKET_DEMON_AI_SET = 0x0181,
  /// Request to interact with a specific bazaar market.
  PACKET_BAZAAR_INTERACT = 0x0184,
  /// Request to forget a specific character skill.
  PACKET_SKILL_FORGET = 0x0186,
  /// Request to stop interacting with a bazaar market.
  PACKET_BAZAAR_MARKET_END = 0x0188,
  /// Request to update the player's bazaar market comment.
  PACKET_BAZAAR_MARKET_COMMENT = 0x018B,
  /// Request replying to a party recruit notification.
  PACKET_PARTY_RECRUIT_REPLY = 0x018F,
  /// Request to add a party member who replied to a recruit notification.
  PACKET_PARTY_RECRUIT = 0x0192,
  /// Request to temporarily change the player character's overhead icon.
  PACKET_STATUS_ICON = 0x0194,
  /// Request to receive map information.
  PACKET_MAP_FLAG = 0x0197,
  /// Request to analyze another player's partner demon.
  PACKET_ANALYZE_DEMON = 0x0199,
  /// Request for the Demon Compendium.
  PACKET_DEMON_COMPENDIUM = 0x019B,
  /// Request to repair an item's max durability.
  PACKET_ITEM_REPAIR_MAX = 0x019F,
  /// Request from the client to alter the appearance of the character.
  PACKET_APPEARANCE_ALTER = 0x01A2,
  /// Request to start a player exchange "entrust" session.
  PACKET_ENTRUST_REQUEST = 0x01AA,
  /// Request to accept an entrust request.
  PACKET_ENTRUST_ACCEPT = 0x01AD,
  /// Request to update the reward items given out upon entrust complete.
  PACKET_ENTRUST_REWARD_UPDATE = 0x01AF,
  /// Request by the entrust target to finish rewards and await confirmation.
  PACKET_ENTRUST_REWARD_FINISH = 0x01B2,
  /// Request by the entrust source to confirm the exchange.
  PACKET_ENTRUST_REWARD_ACCEPT = 0x01B4,
  /// Request by either entrust player to end the exchange in its current state.
  PACKET_ENTRUST_FINISH = 0x01B6,
  /// Request to update the item used for demon crystallization.
  PACKET_DEMON_CRYSTALLIZE_ITEM_UPDATE = 0x01B8,
  /// Request to perform a demon crystallization.
  PACKET_DEMON_CRYSTALLIZE = 0x01BB,
  /// Request to update an item used for enchantment.
  PACKET_ENCHANT_ITEM_UPDATE = 0x01BE,
  /// Request to perform an enchantment.
  PACKET_ENCHANT = 0x01C1,
  /// Request for the current player's dungeon challenge records.
  PACKET_DUNGEON_RECORDS = 0x01C4,
  /// Request for a different player's dungeon challenge records.
  PACKET_ANALYZE_DUNGEON_RECORDS = 0x01C6,
  /// Request to submit an item promo code.
  PACKET_ITEM_PROMO = 0x01C9,
  /// Request to join a tri-fusion session in progress.
  PACKET_TRIFUSION_JOIN = 0x01CD,
  /// Request to update the demons involved in a tri-fusion.
  PACKET_TRIFUSION_DEMON_UPDATE = 0x01D0,
  /// Request to update the rewards given for a tri-fusion success.
  PACKET_TRIFUSION_REWARD_UPDATE = 0x01D3,
  /// Request to accept or reject the rewards being set for a tri-fusion.
  PACKET_TRIFUSION_REWARD_ACCEPT = 0x01D6,
  /// Request to accept (or execute) a tri-fusion.
  PACKET_TRIFUSION_ACCEPT = 0x01D9,
  /// Request to leave a tri-fusion session.
  PACKET_TRIFUSION_LEAVE = 0x01DE,
  /// Request to update the player character's clan's emblem.
  PACKET_CLAN_EMBLEM_UPDATE = 0x01E1,
  /// Request to sync the familiarity of every demon in the player's COMP.
  PACKET_DEMON_FAMILIARITY = 0x01E6,
  /// Request to start the plasma picking minigame for a specific point.
  PACKET_PLASMA_START = 0x01EB,
  /// Request containing the outcome to the plasma picking minigame.
  PACKET_PLASMA_RESULT = 0x01ED,
  /// Request to end plasma picking for a specific point.
  PACKET_PLASMA_END = 0x01EF,
  /// Request for item data for a plasma point.
  PACKET_PLASMA_ITEM_DATA = 0x01F1,
  /// Request to get an item from a plasma point.
  PACKET_PLASMA_ITEM = 0x01F3,
  /// Request sync the current time limit time.
  PACKET_TIME_LIMIT_SYNC = 0x01F9,
  /// Request to disassemble an item for materials.
  PACKET_ITEM_DISASSEMBLE = 0x01FC,
  /// Request from the client to set a weapon synthesis recipe.
  PACKET_SYNTHESIZE_RECIPE = 0x01FE,
  /// Request from the client to perform a weapon synthesis.
  PACKET_SYNTHESIZE = 0x0200,
  /// Request to modify an equipment item slot.
  PACKET_EQUIPMENT_MODIFY = 0x0203,
  /// Request for info about the materials container.
  PACKET_MATERIAL_BOX = 0x0205,
  /// Request to analyze another player character.
  PACKET_ANALYZE = 0x0209,
  /// Request to extract materials from the container.
  PACKET_MATERIAL_EXTRACT = 0x020C,
  /// Request to insert materials into the container.
  PACKET_MATERIAL_INSERT = 0x020E,
  /// Request to exchange an item for something else.
  PACKET_ITEM_EXCHANGE = 0x0210,
  /// Request to open the COMP shop menu.
  PACKET_COMP_SHOP_OPEN = 0x0212,
  /// Request to list all the available COMP shops.
  PACKET_COMP_SHOP_LIST = 0x0214,
  /// Request for the player's fusion gauge state.
  PACKET_FUSION_GAUGE = 0x0217,
  /// Request for the list of available titles.
  PACKET_TITLE_LIST = 0x021B,
  /// Request to update the the character's active title.
  PACKET_TITLE_ACTIVE_UPDATE = 0x021D,
  /// Request to build a new character title.
  PACKET_TITLE_BUILD = 0x0220,
  /// Request to retrieve a pending demon quest from the COMP.
  PACKET_DEMON_QUEST_DATA = 0x0224,
  /// Request to accept a pending demon quest.
  PACKET_DEMON_QUEST_ACCEPT = 0x0226,
  /// Request to turn in the active demon quest.
  PACKET_DEMON_QUEST_END = 0x0229,
  /// Request to cancel the active demon quest.
  PACKET_DEMON_QUEST_CANCEL = 0x022B,
  /// Request for the player's partner demon quest list.
  PACKET_DEMON_QUEST_LIST = 0x022D,
  /// Request for information about the active demon quest.
  PACKET_DEMON_QUEST_ACTIVE = 0x022F,
  /// Contains character confirmation and client graphics settings.
  PACKET_PLAYER_SETTINGS = 0x0232,
  /// Request to lock or unlock a demon in the COMP.
  PACKET_DEMON_LOCK = 0x0233,
  /// Request to reunion the summoned partner demon.
  PACKET_DEMON_REUNION = 0x0235,
  /// Request to reject a pending demon quest.
  PACKET_DEMON_QUEST_REJECT = 0x023A,
  /// Request to confirm a PvP match queue invitation.
  PACKET_PVP_CONFIRM = 0x023E,
  /// Request to start capturing a PvP base.
  PACKET_PVP_BASE_CAPTURE = 0x0242,
  /// Request to stop capturing a PvP base.
  PACKET_PVP_BASE_LEAVE = 0x0247,
  /// Request to join a PvP match queue.
  PACKET_PVP_JOIN = 0x0249,
  /// Request to quit the current PvP match queue.
  PACKET_PVP_CANCEL = 0x024B,
  /// Request for PvP character information.
  PACKET_PVP_CHARACTER_INFO = 0x024D,
  /// Unused. Request for some sort of PvP world information.
  PACKET_PVP_WORLD = 0x024F,
  /// Request to update the character's auto-recovery settings.
  PACKET_AUTO_RECOVERY_UPDATE = 0x025A,
  /// Request to mix two items into a different result item.
  PACKET_ITEM_MIX = 0x025D,
  /// Request to start boosting on a bike.
  PACKET_BIKE_BOOST_ON = 0x0260,
  /// Request to stop boosting on a bike.
  PACKET_BIKE_BOOST_OFF = 0x0262,
  /// Request to dismount from a bike.
  PACKET_BIKE_DISMOUNT = 0x0264,
  /// Request to form a new team.
  PACKET_TEAM_FORM = 0x0267,
  /// Request to invite a player to join the player's team.
  PACKET_TEAM_INVITE = 0x0269,
  /// Request to either accept or reject a team invite.
  PACKET_TEAM_ANSWER = 0x026C,
  /// Request to kick a player from the player's team.
  PACKET_TEAM_KICK = 0x0270,
  /// Request to update the player's current team leader.
  PACKET_TEAM_LEADER_UPDATE = 0x0273,
  /// Request to have the player leave their current team.
  PACKET_TEAM_LEAVE = 0x0276,
  /// Request to send a team channel chat message.
  PACKET_TEAM_CHAT = 0x0279,
  /// Request for the current player's team information.
  PACKET_TEAM_INFO = 0x027B,
  /// Request to list all members in the player's team.
  PACKET_TEAM_MEMBER_LIST = 0x027D,
  /// Unused. Request to update some type of player team info.
  PACKET_TEAM_INFO_UPDATE = 0x027F,
  /// Request to perform equipment spirit fusion.
  PACKET_EQUIPMENT_SPIRIT_FUSE = 0x0287,
  /// Empty message sent after character/demon data requested have been
  /// received.
  PACKET_RECEIVED_PLAYER_DATA = 0x028C,
  /// Empty message sent after setup lists requested have been received.
  PACKET_RECEIVED_LISTS = 0x028E,
  /// Request to re-send a pending demon quest.
  PACKET_DEMON_QUEST_PENDING = 0x028F,
  /// Request to open the remote item depos.
  PACKET_ITEM_DEPO_REMOTE = 0x0296,
  /// Request to capture a Diaspora instance base.
  PACKET_DIASPORA_BASE_CAPTURE = 0x02C4,
  /// Request to enter a Diaspora instance after team establishment.
  PACKET_DIASPORA_ENTER = 0x02C7,
  /// Request to open the remote demon depo.
  PACKET_DEMON_DEPO_REMOTE = 0x02EF,
  /// Request to update character common switches.
  PACKET_COMMON_SWITCH_UPDATE = 0x02F2,
  /// Request for character common switch settings.
  PACKET_COMMON_SWITCH_INFO = 0x02F4,
  /// Request to consume a demon force item.
  PACKET_DEMON_FORCE = 0x02F6,
  /// Request to add or remove the pending force stack effect.
  PACKET_DEMON_FORCE_STACK = 0x02F8,
  /// Request for the current character's casino coin total.
  PACKET_CASINO_COIN_TOTAL = 0x02FA,
  /// Request to perform a solo tri-fusion.
  PACKET_TRIFUSION_SOLO = 0x0384,
  /// Request to reverse equipment spirit fusion.
  PACKET_EQUIPMENT_SPIRIT_DEFUSE = 0x0386,
  /// Request to complete demon force changes.
  PACKET_DEMON_FORCE_END = 0x0389,
  /// Request to spectate a target character in a UB match.
  PACKET_UB_SPECTATE_PLAYER = 0x038F,
  /// Request to proceed past a UB round result display.
  PACKET_UB_PROCEED = 0x0392,
  /// Request to leave a UB instance prematurely.
  PACKET_UB_LEAVE = 0x0394,
  /// Request to cancel the client's UB lotto queue entry.
  PACKET_UB_LOTTO_CANCEL = 0x0398,
  /// Request to create a UB lotto queue entry.
  PACKET_UB_LOTTO_JOIN = 0x039A,
  /// Request for the current player's search entries.
  PACKET_SEARCH_ENTRY_INFO = 0x03A3,
  /// Request for the current player's I-Time data.
  PACKET_ITIME_DATA = 0x03A5,
  /// Request to start or continue an I-Time conversation.
  PACKET_ITIME_TALK = 0x03A9,
  /// Request for the current player's item culture data.
  PACKET_CULTURE_DATA = 0x03AC,
  /// Request to access a culture machine being interacted with.
  PACKET_CULTURE_MACHINE_ACCESS = 0x03AE,
  /// Request to start using a culture machine.
  PACKET_CULTURE_START = 0x03B0,
  /// Request to add an item to increase culture machine success.
  PACKET_CULTURE_ITEM = 0x03B2,
  /// Request to end interaction with a culture machine.
  PACKET_CULTURE_END = 0x03B6,
  /// Request to edit a previously applied equipment modification.
  PACKET_EQUIPMENT_MOD_EDIT = 0x03CB,
  /// Request for the next punitive attribute deadline.
  PACKET_PATTRIBUTE_DEADLINE = 0x03EA,
  /// Request to leave a Mission instance.
  PACKET_MISSION_LEAVE = 0x03ED,
  /// Unknown. Request to end some type of match (possibly depracated).
  PACKET_GAME_OVER = 0x03EF,
  /// Request to perform a mitama reunion reinforcement.
  PACKET_MITAMA_REUNION = 0x03F1,
  /// Request to reset a mitama reunion growth path.
  PACKET_MITAMA_RESET = 0x03F3,
  /// Request to list the client account's demon depositories.
  PACKET_DEMON_DEPO_LIST = 0x03F5,
  /// Request to change demon equipment.
  PACKET_DEMON_EQUIP = 0x03FB,
  /// Request to process the results of an NPC barter.
  PACKET_BARTER = 0x03FE,
  /// Request for the current world and character Pentalpha data.
  PACKET_PENTALPHA_DATA = 0x0400,
  /// Request to obtain a quest bonus title.
  PACKET_QUEST_TITLE = 0x0406,
  /// Request report a player for abuse.
  PACKET_REPORT_PLAYER = 0x0407,
  /// Request for the current player's blacklist.
  PACKET_BLACKLIST = 0x0408,
  /// Request to update the current player's blacklist.
  PACKET_BLACKLIST_UPDATE = 0x040A,
  /// Request for Destiny box data.
  PACKET_DESTINY_BOX_DATA = 0x040D,
  /// Request to start a Destiny box item lotto.
  PACKET_DESTINY_LOTTO = 0x0410,
  /// Request for the current player's digitalize point information.
  PACKET_DIGITALIZE_POINTS = 0x0414,
  /// Request for the current player's digitalize assist information.
  PACKET_DIGITALIZE_ASSIST = 0x0418,
  /// Request to learn a digitalize assist skill.
  PACKET_DIGITALIZE_ASSIST_LEARN = 0x041A,
  /// Request to remove an active digitalize assist skill.
  PACKET_DIGITALIZE_ASSIST_REMOVE = 0x041C,
  /// Request to list all VA items in the VA closet.
  PACKET_VA_BOX = 0x041E,
  /// Request to create a VA item and add it to the closet.
  PACKET_VA_BOX_ADD = 0x0420,
  /// Request to remove a VA item from the closet.
  PACKET_VA_BOX_REMOVE = 0x0422,
  /// Request to change the current character's VA.
  PACKET_VA_CHANGE = 0x0424,
  /// Request to move a VA item in the closet.
  PACKET_VA_BOX_MOVE = 0x0427,
  /// Request to retrieve the player's reunion conversion points.
  PACKET_REUNION_POINTS = 0x0432,
  /// Request to extract reunion conversion points from a demon.
  PACKET_REUNION_EXTRACT = 0x0434,
  /// Request to inject reunion conversion points into a demon.
  PACKET_REUNION_INJECT = 0x0436,
  /// Unused and never seen, possibly a debug packet.
  PACKET_UNUSED_FFEB = 0xFFEB,
  /// Unused and never seen, possibly a debug packet.
  PACKET_UNUSED_FFEC = 0xFFEC,
  /// Unused and never seen, possibly a debug packet.
  PACKET_UNUSED_FFED = 0xFFED,

  //
  // Custom Packets (Amala Network)
  //
  /// Request a dump of the account data.
  PACKET_AMALA_REQ_ACCOUNT_DUMP = 0x1000,
};

/**
 * Response packet code from the channel server to the game client.
 */
enum class ChannelToClientPacketCode_t : uint16_t {
  /// Login response.
  PACKET_LOGIN = 0x0001,
  /// Authorization response.
  PACKET_AUTH = 0x0003,
  /// Logout response.
  PACKET_LOGOUT = 0x0009,
  /// Message containing data about the client's character.
  PACKET_CHARACTER_DATA = 0x000F,
  /// Message containing data about the client's active demon.
  PACKET_PARTNER_DATA = 0x0010,
  /// Message containing data about an enemy in the zone.
  PACKET_ENEMY_DATA = 0x0011,
  /// Message containing data about a lootable enemy body in the zone.
  PACKET_LOOT_BODY_DATA = 0x0012,
  /// Message containing data about a loot box in the zone.
  PACKET_LOOT_BOX_DATA = 0x0013,
  /// Message containing data about an NPC in a zone.
  PACKET_NPC_DATA = 0x0014,
  /// Message containing data about an object NPC in the zone.
  PACKET_OBJECT_NPC_DATA = 0x0015,
  /// Message containing data about a different client's character.
  PACKET_OTHER_CHARACTER_DATA = 0x0016,
  /// Message containing data about a different client's active demon.
  PACKET_OTHER_PARTNER_DATA = 0x0017,
  /// Message to remove an entity or object that belongs to the client.
  PACKET_REMOVE_OBJECT = 0x0018,
  /// Message to display a game entity.
  PACKET_SHOW_ENTITY = 0x001A,
  /// Message to remove a game entity.
  PACKET_REMOVE_ENTITY = 0x001B,
  /// Message containing entity or object movement information.
  PACKET_MOVE = 0x001D,
  /// Unused. Skill independent post-skill entity knock back effect.
  PACKET_KNOCK_BACK = 0x001E,
  /// Unused. Skill independent post-skill entity "stiffness" effect.
  PACKET_STIFFEN_OBJECT = 0x001F,
  /// Message forcing an entity to stay in a set location for a set time.
  PACKET_FIX_POSITION = 0x0021,
  /// Message containing entity or object warping information.
  PACKET_WARP = 0x0022,
  /// Information about a character's zone.
  PACKET_ZONE_CHANGE = 0x0023,
  /// Message containing the current server time.
  PACKET_SYNC_TIME = 0x0025,
  /// Response from chat request.
  PACKET_CHAT = 0x0028,
  /// Notification that a skill has been activated.
  PACKET_SKILL_ACTIVATED = 0x0033,
  /// Notification that a skill has completed execution.
  PACKET_SKILL_COMPLETED = 0x0034,
  /// Notification that a skill has executed once.
  PACKET_SKILL_EXECUTED = 0x0036,
  /// Notification that a skill's execution has failed.
  PACKET_SKILL_FAILED = 0x0037,
  /// Message with information about entities affected by a skill's execution.
  PACKET_SKILL_REPORTS = 0x0038,
  /// Notification that a switch skill has been enabled or disabled.
  PACKET_SKILL_SWITCH = 0x0039,
  /// Notification that an entity's MP has dropped from skill upkeep cost.
  PACKET_SKILL_UPKEEP_COST = 0x003A,
  /// Notifies the client that an entity has entered into a battle.
  PACKET_BATTLE_STARTED = 0x003B,
  /// Notifies the client that an entity is no longer in battle.
  PACKET_BATTLE_STOPPED = 0x003C,
  /// Unused. Notifies the client that an entity has retargeted a skill.
  PACKET_SKILL_TARGETED = 0x003E,
  /// Notifies the client that an enemy has become activate.
  PACKET_ENEMY_ACTIVATED = 0x0041,
  /// Notifies the client that "time" damage has been dealt to an entity.
  PACKET_DO_TDAMAGE = 0x0043,
  /// Notifies the client that an entity has gained a status effect.
  PACKET_ADD_STATUS_EFFECT = 0x0044,
  /// Notifies the client that an entity has lost a status effect.
  PACKET_REMOVE_STATUS_EFFECT = 0x0045,
  /// Notifies the client of an entity's XP value.
  PACKET_XP_UPDATE = 0x0046,
  /// Notifies the client that a character has leveled up.
  PACKET_CHARACTER_LEVEL_UP = 0x0047,
  /// Notifies the client that a partner demon has leveled up.
  PACKET_PARTNER_LEVEL_UP = 0x0048,
  /// Response from the request to allocate a skill point for a character.
  PACKET_ALLOCATE_SKILL_POINT = 0x004A,
  /// Notifies the client that a character's equipment has changed.
  PACKET_EQUIPMENT_CHANGED = 0x004B,
  /// Notifies the client of a character or parter demon's calculated stats.
  PACKET_ENTITY_STATS = 0x004C,
  /// Notifies the client that a character's expertise points have been updated.
  PACKET_EXPERTISE_POINT_UPDATE = 0x004D,
  /// Notifies the client that a character's expertise has ranked up.
  PACKET_EXPERTISE_RANK_UP = 0x004E,
  /// Notifies the client to enable or disable an expertise.
  PACKET_TOGGLE_EXPERTISE = 0x0050,
  /// Notifies the client that a character has learned a skill.
  PACKET_LEARN_SKILL = 0x0052,
  /// Notifies the client that the parner demon's inherited skills have been
  /// updated.
  PACKET_INHERIT_SKILL_UPDATED = 0x0053,
  /// Notifies the client that the partner demon's learned skill set has been
  /// updated.
  PACKET_DEMON_SKILL_UPDATE = 0x0055,
  /// Response to keep the client connection alive.
  PACKET_KEEP_ALIVE = 0x0057,
  /// Response to fix a game object's position.
  PACKET_FIX_OBJECT_POSITION = 0x0059,
  /// Demon box list response.
  PACKET_DEMON_BOX = 0x005D,
  /// Demon box demon data response.
  PACKET_DEMON_BOX_DATA = 0x005F,
  /// Notifies the client that a partner demon has been summoned.
  PACKET_PARTNER_SUMMONED = 0x0060,
  /// Sets up an entity that will spawn in via PACKET_SHOW_ENTITY.
  PACKET_POP_ENTITY_FOR_PRODUCTION = 0x0061,
  /// Message containing the list of channels connected to the world.
  PACKET_CHANNEL_LIST = 0x0064,
  /// Response to the request to change channels.
  PACKET_CHANNEL_CHANGE = 0x0066,
  /// Notification that an entity has been revived.
  PACKET_REVIVE_ENTITY = 0x006E,
  /// Message containing entity or object movement stopping information.
  PACKET_STOP_MOVEMENT = 0x0070,
  /// Response for the server's current world time.
  PACKET_WORLD_TIME = 0x0073,
  /// Response for info about a specific item box.
  PACKET_ITEM_BOX = 0x0075,
  /// Notification that an item request packet has failed. Causes an item box
  /// refresh.
  PACKET_ERROR_ITEM = 0x0079,
  /// Message containing updated information about an item in a box.
  PACKET_ITEM_UPDATE = 0x007A,
  /// Response for equipment information.
  PACKET_EQUIPMENT_LIST = 0x007C,
  /// Response for starting a trade with another player.
  PACKET_TRADE_REQUEST = 0x007E,
  /// Notification to another player that a trade is being requested.
  PACKET_TRADE_REQUESTED = 0x007F,
  /// Response for accepting the trade request from another player.
  PACKET_TRADE_ACCEPT = 0x0081,
  /// Response for adding an item to the current trade offer.
  PACKET_TRADE_ADD_ITEM = 0x0083,
  /// Notification than an item has been added to the current trade offer.
  PACKET_TRADE_ADDED_ITEM = 0x0084,
  /// Response for locking the current trade for acceptance.
  PACKET_TRADE_LOCK = 0x0086,
  /// Notification that the other player has locked their current trade offer.
  PACKET_TRADE_LOCKED = 0x0087,
  /// Response for finishing the current trade.
  PACKET_TRADE_FINISH = 0x0089,
  /// Notification that the current trade has finished.
  PACKET_TRADE_FINISHED = 0x008A,
  /// Notification that the current trade has ended and wheter it was a success
  /// of failure.
  PACKET_TRADE_ENDED = 0x008C,
  /// Message containing the list of items inside of a loot box.
  PACKET_LOOT_ITEM_DATA = 0x008D,
  /// Response to the request to loot an item from a loot box.
  PACKET_LOOT_ITEM = 0x008F,
  /// Response containing the current account's total CP.
  PACKET_CASH_BALANCE = 0x0091,
  /// Response containing details about an NPC shop.
  PACKET_SHOP_DATA = 0x0093,
  /// Response to the request to buy an item from a shop.
  PACKET_SHOP_BUY = 0x0095,
  /// Response to the request to sell an item to a shop.
  PACKET_SHOP_SELL = 0x0097,
  /// Message containing information that one or more demon box has been
  /// updated.
  PACKET_DEMON_BOX_UPDATE = 0x0098,
  /// Notification that a demon box request packet has failed. Causes a demon
  /// box refresh.
  PACKET_ERROR_COMP = 0x0099,
  /// Response to the request to list items in the player account's Post.
  PACKET_POST_LIST = 0x009D,
  /// Response to the request to move an item from the Post into the current
  /// character's inventory.
  PACKET_POST_ITEM = 0x009F,
  /// Response to the request to open/retrieve post gift information.
  PACKET_POST_GIFT = 0x00A1,
  /// Response for data about a hotbar page.
  PACKET_HOTBAR_DATA = 0x00A3,
  /// Response to save a hotbar page.
  PACKET_HOTBAR_SAVE = 0x00A5,
  /// Request to the client to display an event message.
  PACKET_EVENT_MESSAGE = 0x00A6,
  /// Request to the client to display an NPC event message.
  PACKET_EVENT_NPC_MESSAGE = 0x00A7,
  /// Request to the client to display choices to the player.
  PACKET_EVENT_PROMPT = 0x00AC,
  /// Request to the client to play an in-game cinematic.
  PACKET_EVENT_PLAY_SCENE = 0x00AD,
  /// Request to the client to open a menu.
  PACKET_EVENT_OPEN_MENU = 0x00AE,
  /// Response containing a list of obtained valuables.
  PACKET_VALUABLE_LIST = 0x00B9,
  /// Request to the client to end the current event.
  PACKET_EVENT_END = 0x00BB,
  /// Notification that an object NPC's state has changed.
  PACKET_NPC_STATE_CHANGE = 0x00BC,
  /// Response containing the current player's own friend information.
  PACKET_FRIEND_INFO_SELF = 0x00BE,
  /// Message containing information about a friend on the friends list.
  PACKET_FRIEND_INFO = 0x00BF,
  /// Response from a player to be added to the player's friend list.
  PACKET_FRIEND_REQUEST = 0x00C1,
  /// Notification that a friend request has been received.
  PACKET_FRIEND_REQUESTED = 0x00C2,
  /// Response to the client stating that a friend was added or removed.
  PACKET_FRIEND_ADD_REMOVE = 0x00C5,
  /// Notification to update friend list specific data from a player.
  PACKET_FRIEND_DATA = 0x00C7,
  /// Response to the request to invite a player to join the player's party.
  PACKET_PARTY_INVITE = 0x00D3,
  /// Notification that a party invite has been received.
  PACKET_PARTY_INVITED = 0x00D4,
  /// Response to the request to join a party.
  PACKET_PARTY_JOIN = 0x00D6,
  /// Message containing information about a party member.
  PACKET_PARTY_MEMBER_INFO = 0x00D7,
  /// Response to the request to cancel a party invite.
  PACKET_PARTY_CANCEL = 0x00D9,
  /// Response to the request to leave the player's current party.
  PACKET_PARTY_LEAVE = 0x00DB,
  /// Notification that a player has left the current party.
  PACKET_PARTY_LEFT = 0x00DC,
  /// Response to the request to disband the current party.
  PACKET_PARTY_DISBAND = 0x00DE,
  /// Notification that the current party has been disbanded.
  PACKET_PARTY_DISBANDED = 0x00DF,
  /// Response to the request to update the current party's designated leader.
  PACKET_PARTY_LEADER_UPDATE = 0x00E1,
  /// Notification that the current party's designated leader has changed.
  PACKET_PARTY_LEADER_UPDATED = 0x00E2,
  /// Response to the request to update the current party's drop rule setting.
  PACKET_PARTY_DROP_RULE = 0x00E4,
  /// Notification that the current party's drop rule has been changed.
  PACKET_PARTY_DROP_RULE_SET = 0x00E5,
  /// Notification containing a player character's info in the current party.
  PACKET_PARTY_MEMBER_UPDATE = 0x00E7,
  /// Notification containing a parter demon's info in the current party.
  PACKET_PARTY_MEMBER_PARTNER = 0x00E8,
  /// Notification that a current party member has moved to a different zone.
  PACKET_PARTY_MEMBER_ZONE = 0x00E9,
  /// Notification that a current party member's icon state has changed.
  PACKET_PARTY_MEMBER_ICON = 0x00EA,
  /// Notification that a player has been kicked from the current party.
  PACKET_PARTY_KICK = 0x00EC,
  /// Response containing the results of a two-way fusion.
  PACKET_DEMON_FUSION = 0x00F0,
  /// Response to the request for information about the demon in a demon egg.
  PACKET_LOOT_DEMON_EGG_DATA = 0x00F2,
  /// Response containing the server time.
  PACKET_SYNC = 0x00F4,
  /// Notification that a friend request packet has failed.
  PACKET_ERROR_FRIEND = 0x00F5,
  /// Response to the request to repair an item at a shop.
  PACKET_SHOP_REPAIR = 0x00F7,
  /// Message containing entity or object rotation information.
  PACKET_ROTATE = 0x00F9,
  /// Message containing an entity's updated running speed.
  PACKET_RUN_SPEED = 0x00FB,
  /// Response to the request for the list of items inside a boss loot box.
  PACKET_LOOT_BOSS_BOX = 0x00FD,
  /// Message containing union information.
  PACKET_UNION_FLAG = 0x0101,
  /// Response containing the list of the client account's item depositories.
  PACKET_ITEM_DEPO_LIST = 0x0103,
  /// Response to renting a client account item or demon depository.
  PACKET_DEPO_RENT = 0x0105,
  /// Message indicating that the player's homepoint has been updated.
  PACKET_EVENT_HOMEPOINT_UPDATE = 0x0106,
  /// Response to the request for the list of items inside a treasure loot box.
  PACKET_LOOT_TREASURE_BOX = 0x0108,
  /// Notification that one of the player character's expertise has been
  /// lowered.
  PACKET_EXPERTISE_DOWN = 0x0109,
  /// Response containing the player's active quest list.
  PACKET_QUEST_ACTIVE_LIST = 0x010D,
  /// Response containing the player's completed quest list.
  PACKET_QUEST_COMPLETED_LIST = 0x010E,
  /// Notification that a quest's phase has been updated.
  PACKET_QUEST_PHASE_UPDATE = 0x010F,
  /// Notification that a quest's kill counts have been updated.
  PACKET_QUEST_KILL_COUNT_UPDATE = 0x0110,
  /// Deprecated signifier that a list of quests is being sent to the client.
  PACKET_QUEST_LIST_START = 0x0111,
  /// Deprecated signifier that a list of quests has finished being sent to the
  /// client.
  PACKET_QUEST_LIST_END = 0x0112,
  /// Response to the request to open a market at a bazaar.
  PACKET_BAZAAR_MARKET_OPEN = 0x0114,
  /// Notification (and response) to close the player's current open bazaar
  /// market.
  PACKET_BAZAAR_MARKET_CLOSE = 0x0116,
  /// Message containing details about a specific bazaar market.
  PACKET_BAZAAR_MARKET_INFO = 0x0118,
  /// Response to the request to add an item to the player's bazaar market.
  PACKET_BAZAAR_ITEM_ADD = 0x011A,
  /// Response to the request to drop an item from the player's bazaar market.
  PACKET_BAZAAR_ITEM_DROP = 0x011C,
  /// Response to the request to update an item in the player's bazaar market.
  PACKET_BAZAAR_ITEM_UPDATE = 0x011E,
  /// Response to the request to buy an item from a bazaar market.
  PACKET_BAZAAR_ITEM_BUY = 0x0120,
  /// Notification that an item in the player's bazaar market has been sold.
  PACKET_BAZAAR_ITEM_SOLD = 0x0121,
  /// Response to the request to take sales macca from the player's bazaar
  /// market.
  PACKET_BAZAAR_MARKET_SALES = 0x0123,
  /// Unknown. Response to the request to exchange one item for another.
  PACKET_BAZAAR_ITEM_EXCHANGE = 0x0125,
  /// Message containing a character's LNC alignment value.
  PACKET_LNC_POINTS = 0x0126,
  /// Request to the client to render a stage effect.
  PACKET_EVENT_STAGE_EFFECT = 0x0127,
  /// Response containing the player's current search entries of a specified
  /// type.
  PACKET_SEARCH_ENTRY_SELF = 0x0129,
  /// Response containing a filtered list of search entries of a specified type.
  PACKET_SEARCH_LIST = 0x012B,
  /// Response containing detailed information about a specific search entry.
  PACKET_SEARCH_ENTRY_DATA = 0x012D,
  /// Response to the request to register a new search entry.
  PACKET_SEARCH_ENTRY_REGISTER = 0x012F,
  /// Response to the request to update an existing search entry.
  PACKET_SEARCH_ENTRY_UPDATE = 0x0131,
  /// Response to the request to remove an existing search entry.
  PACKET_SEARCH_ENTRY_REMOVE = 0x0133,
  /// Notification that a search entry application has been modified.
  PACKET_SEARCH_APPLICATION = 0x0134,
  /// Response to the request to perform an action based on a search entry
  /// application.
  PACKET_SEARCH_APPLICATION_REPLY = 0x0136,
  /// Response to the request to form a new clan from a clan formation item.
  PACKET_CLAN_FORM = 0x0137,
  /// Response to the request to disband the player character's clan.
  PACKET_CLAN_DISBAND = 0x0139,
  /// Notification that the player character's clan has disbanded.
  PACKET_CLAN_DISBANDED = 0x013A,
  /// Response to the request to invite a character to the player character's
  /// clan.
  PACKET_CLAN_INVITE = 0x013C,
  /// Notification that the player character has been invited to join a clan.
  PACKET_CLAN_INVITED = 0x013D,
  /// Response to the request to accept an invite to join a clan.
  PACKET_CLAN_JOIN = 0x013F,
  /// Response to the request to reject an invite to join a clan.
  PACKET_CLAN_CANCEL = 0x0141,
  /// Response to the request to kick a character from the player character's
  /// clan.
  PACKET_CLAN_KICK = 0x0143,
  /// Notification that a character has been kicked from the player character's
  /// clan.
  PACKET_CLAN_KICKED = 0x0144,
  /// Response to the request to update a clan's master member.
  PACKET_CLAN_MASTER_UPDATE = 0x0146,
  /// Notification that the player character's clan's master has changed.
  PACKET_CLAN_MASTER_UPDATED = 0x0147,
  /// Response to the request to toggle a player's sub-master member type.
  PACKET_CLAN_SUB_MASTER_UPDATE = 0x0149,
  /// Notification that a clan member has had their sub-master role toggled.
  PACKET_CLAN_SUB_MASTER_UPDATED = 0x014A,
  /// Response to the request to leave the player character's clan.
  PACKET_CLAN_LEAVE = 0x014C,
  /// Notification that a member of the player character's clan has left.
  PACKET_CLAN_LEFT = 0x014D,
  /// Chat message sent from a member of the same clan.
  PACKET_CLAN_CHAT = 0x014F,
  /// Response containing the current player's clan information.
  PACKET_CLAN_INFO = 0x0151,
  /// Response to the request for member information from the current player
  /// character's clan.
  PACKET_CLAN_LIST = 0x0153,
  /// Notification that a player character's clan's member info has updated
  PACKET_CLAN_DATA = 0x0155,
  /// Response containing an item price estimate based off several criteria.
  PACKET_ITEM_PRICE = 0x0158,
  /// Notification of the client state relative to the "comm server" (world).
  PACKET_COMM_SERVER_STATE = 0x015A,
  /// Request to the client to signify a direction to the player.
  PACKET_EVENT_DIRECTION = 0x015D,
  /// Unused. Notification that a player has received an "anti-monopoly" drop
  /// demerit.
  PACKET_TROPHY_PENALTY = 0x015E,
  /// Response to the request for the current zone's bazaar cost and duration.
  PACKET_BAZAAR_STATE = 0x0160,
  /// Response to the request to set the player's bazaar clerk NPC.
  PACKET_BAZAAR_CLERK_SET = 0x0162,
  /// Unknown. Notification (to?) that all items have sold out for a bazaar
  /// market.
  PACKET_BAZAAR_SOLD_OUT = 0x0163,
  /// Response to the request to get a suggested sales price for a bazaar item.
  PACKET_BAZAAR_PRICE = 0x0165,
  /// Message containing details about a the player's bazaar market.
  PACKET_BAZAAR_MARKET_INFO_SELF = 0x0167,
  /// Notification that a character's clan name has updated.
  PACKET_CLAN_NAME_UPDATED = 0x0169,
  /// Notification that a skill is executing, bypassing normal activation.
  PACKET_SKILL_EXECUTED_INSTANT = 0x016C,
  /// Notification that the player character's skill points have been reset.
  PACKET_RESET_SKILL_POINTS = 0x0170,
  /// Message containing announcement ticker data.
  PACKET_SYSTEM_MSG = 0x0171,
  /// Response to the request to sync a client entity's basic information.
  PACKET_SYNC_CHARACTER = 0x017F,
  /// Unknown. Your guess is as good as mine.
  PACKET_ASSORT_DEVELOP = 0x0180,
  /// Notifies the client that their COMP size has changed.
  PACKET_COMP_SIZE_UPDATED = 0x0182,
  /// Message containing data about a bazaar in a zone.
  PACKET_BAZAAR_DATA = 0x0183,
  /// Notification that a bazaar market clerk NPC has changed.
  PACKET_BAZAAR_NPC_CHANGED = 0x0185,
  /// Notification that a player's skill list has been updated.
  PACKET_SKILL_LIST_UPDATED = 0x0187,
  /// Response to the request to update the player's bazaar market comment.
  PACKET_BAZAAR_MARKET_COMMENT = 0x018C,
  /// Notification that a player has contracted a demon from a demon egg.
  PACKET_CONTRACT_COMPLETED = 0x018D,
  /// Response to the request replying to a party recruit notification.
  PACKET_PARTY_RECRUIT_REPLY = 0x0190,
  /// Notification that a player has replied to the client's party recruit
  /// notification.
  PACKET_PARTY_RECRUIT_REPLIED = 0x0191,
  /// Response to the request to add a party member who replied to a recruit
  /// notification.
  PACKET_PARTY_RECRUIT = 0x0193,
  /// Message containing the icon to show for the client's character.
  PACKET_STATUS_ICON = 0x0195,
  /// Message containing the icon to show for another player's character.
  PACKET_STATUS_ICON_OTHER = 0x0196,
  /// Message containing map information.
  PACKET_MAP_FLAG = 0x0198,
  /// Response to the request to analyze another player's partner demon.
  PACKET_ANALYZE_DEMON = 0x019A,
  /// Response containing the Demon Compendium.
  PACKET_DEMON_COMPENDIUM = 0x019C,
  /// Notification that the Demon Compendium has been updated.
  PACKET_DEMON_COMPENDIUM_ADD = 0x019D,
  /// Notification that an entity's skill costs have been modified.
  PACKET_SKILL_COSTS = 0x019E,
  /// Response to the request to repair an item's max durability.
  PACKET_ITEM_REPAIR_MAX = 0x01A0,
  /// Response to the request from the client to alter the appearance of the
  /// character.
  PACKET_APPEARANCE_ALTER = 0x01A3,
  /// Notification that a character has altered their appearance.
  PACKET_APPEARANCE_ALTERED = 0x01A4,
  /// Notification that the current partner demon's familiarity has updated.
  PACKET_DEMON_FAMILIARITY_UPDATE = 0x01A5,
  /// Unused. Notification that a time trial has started (replaced by update).
  PACKET_TIME_TRIAL_START = 0x01A6,
  /// Notification that a time trial zone instance has been entered.
  PACKET_TIME_TRIAL_UPDATE = 0x01A7,
  /// Notification that a time trial zone instance has ended.
  PACKET_TIME_TRIAL_END = 0x01A8,
  /// Notification that a time trial record has been turned in.
  PACKET_TIME_TRIAL_REPORT = 0x01A9,
  /// Response to the request to start a player exchange "entrust" session.
  PACKET_ENTRUST_REQUEST = 0x01AB,
  /// Notification for an entrust target that a session is being requested.
  PACKET_ENTRUST_REQUESTED = 0x01AC,
  /// Response to the request to accept an entrust request.
  PACKET_ENTRUST_ACCEPT = 0x01AE,
  /// Response to the request to update the reward items given out upon entrust
  /// complete.
  PACKET_ENTRUST_REWARD_UPDATE = 0x01B0,
  /// Notification to entrust players that a reward has been updated.
  PACKET_ENTRUST_REWARD_UPDATED = 0x01B1,
  /// Notification that the entrust target has finished setting rewards for
  /// confirmation.
  PACKET_ENTRUST_REWARD_FINISH = 0x01B3,
  /// Notification that the entrust source has accepted the exchange.
  PACKET_ENTRUST_REWARD_ACCEPT = 0x01B5,
  /// Notification that the entrust exchange session has ended.
  PACKET_ENTRUST_FINISH = 0x01B7,
  /// Response to the request to update the item used for demon crystallization.
  PACKET_DEMON_CRYSTALLIZE_ITEM_UPDATE = 0x01B9,
  /// Notification that the item used for demon crystallization has been
  /// updated.
  PACKET_DEMON_CRYSTALLIZE_ITEM_UPDATED = 0x01BA,
  /// Response to the request to perform a demon crystallization.
  PACKET_DEMON_CRYSTALLIZE = 0x01BC,
  /// Notification that a demon crystallization has taken place.
  PACKET_DEMON_CRYSTALLIZED = 0x01BD,
  /// Response to the request to update an item used for enchantment.
  PACKET_ENCHANT_ITEM_UPDATE = 0x01BF,
  /// Notification that an item used for enchantment has been updated.
  PACKET_ENCHANT_ITEM_UPDATED = 0x01C0,
  /// Response to the request to perform an enchantment.
  PACKET_ENCHANT = 0x01C2,
  /// Notification that an enchantment has been performed.
  PACKET_ENCHANTED = 0x01C3,
  /// Response containing the current player's dungeon challenge records.
  PACKET_DUNGEON_CHALLENGES = 0x01C5,
  /// Response to the request for a different player's dungeon challenge
  /// records.
  PACKET_ANALYZE_DUNGEON_RECORDS = 0x01C7,
  /// Notification that the player's dungeon challenge records have been
  /// updated.
  PACKET_DUNGEON_RECORDS_UPDATE = 0x01C8,
  /// Response to the request to submit an item promo code.
  PACKET_ITEM_PROMO = 0x01CA,
  /// Notification that a party member in the same zone has started a tri-fusion
  /// session.
  PACKET_TRIFUSION_STARTED = 0x01CB,
  /// Request to the client to start a tri-fusion session.
  PACKET_TRIFUSION_START = 0x01CC,
  /// Response to the request to join a tri-fusion session in progress.
  PACKET_TRIFUSION_JOIN = 0x01CE,
  /// Message containing information about a character joining a tri-fusion
  /// session.
  PACKET_TRIFUSION_PARTICIPANT = 0x01CF,
  /// Response to the request to update the demons involved in a tri-fusion.
  PACKET_TRIFUSION_DEMON_UPDATE = 0x01D1,
  /// Notification that the demons involved in a tri-fusion have been updated.
  PACKET_TRIFUSION_DEMON_UPDATED = 0x01D2,
  /// Response to the request to update the rewards given for a tri-fusion
  /// success.
  PACKET_TRIFUSION_REWARD_UPDATE = 0x01D4,
  /// Notification that the rewards for a tri-fusion success have been updated.
  PACKET_TRIFUSION_REWARD_UPDATED = 0x01D5,
  /// Response to the request to accept or reject the rewards being set for a
  /// tri-fusion.
  PACKET_TRIFUSION_REWARD_ACCEPT = 0x01D7,
  /// Notification that the rewards being set for a tri-fusion have been acceped
  /// or rejected.
  PACKET_TRIFUSION_REWARD_ACCEPTED = 0x01D8,
  /// Response to the request to accept (or execute) a tri-fusion.
  PACKET_TRIFUSION_ACCEPT = 0x01DA,
  /// Notification that the tri-fusion has been accepted by a player
  PACKET_TRIFUSION_ACCEPTED = 0x01DB,
  /// Message containing the result of a normal tri-fusion.
  PACKET_TRIFUSION = 0x01DC,
  /// Notification that a demon invovled in a tri-fusion has been updated.
  PACKET_TRIFUSION_UPDATE = 0x01DD,
  /// Notification that a character has left the tri-fusion session.
  PACKET_TRIFUSION_LEFT = 0x01DF,
  /// Notification that a tri-fusion session has ended.
  PACKET_TRIFUSION_END = 0x01E0,
  /// Response to the request to update the player character's clan's emblem.
  PACKET_CLAN_EMBLEM_UPDATE = 0x01E2,
  /// Notification that a character's clan emblem has updated.
  PACKET_CLAN_EMBLEM_UPDATED = 0x01E3,
  /// Notification that a character's clan level has updated.
  PACKET_CLAN_LEVEL_UPDATED = 0x01E4,
  /// Notification to a character that their visible clan info has been updated.
  PACKET_CLAN_UPDATE = 0x01E5,
  /// Response to the request for the familiarity values of each demon in the
  /// COMP.
  PACKET_DEMON_FAMILIARITY = 0x01E7,
  /// Notification that a successful tri-fusion has resulted in a demon crystal.
  PACKET_TRIFUSION_DEMON_CRYSTAL = 0x01E8,
  /// Request to the client to display an extended NPC event message.
  PACKET_EVENT_EX_NPC_MESSAGE = 0x01E9,
  /// Message containing data about a plasma spawn in a zone.
  PACKET_PLASMA_DATA = 0x01EA,
  /// Response to the request to start the plasma picking minigame for a
  /// specific point.
  PACKET_PLASMA_START = 0x01EC,
  /// Response to the request containing the outcome to the plasma picking
  /// minigame.
  PACKET_PLASMA_RESULT = 0x01EE,
  /// Notification that plasma picking has ended for a specific point.
  PACKET_PLASMA_END = 0x01F0,
  /// Response to the request for item data for a plasma point.
  PACKET_PLASMA_ITEM_DATA = 0x01F2,
  /// Response to the request to get an item from a plasma point.
  PACKET_PLASMA_ITEM = 0x01F4,
  /// Notification that a plasma spawn's status has been updated.
  PACKET_PLASMA_STATUS = 0x01F5,
  /// Notification that one or more plasma point has been updated.
  PACKET_PLASMA_REPOP = 0x01F6,
  /// Notification that the client's zone instance has a set time limit.
  PACKET_TIME_LIMIT_UPDATE = 0x01F7,
  /// Notification that the client's zone instance time limit has ended.
  PACKET_TIME_LIMIT_END = 0x01F8,
  /// Response to the request sync the current time limit time.
  PACKET_TIME_LIMIT_SYNC = 0x01FA,
  /// Request to the client to play a sound effect as part of an event.
  PACKET_EVENT_PLAY_SOUND_EFFECT = 0x01FB,
  /// Response to the request to disassemble an item for materials.
  PACKET_ITEM_DISASSEMBLE = 0x01FD,
  /// Response to the request from the client to set a weapon synthesis recipe.
  PACKET_SYNTHESIZE_RECIPE = 0x01FF,
  /// Response to the request from the client to perform a weapon synthesis.
  PACKET_SYNTHESIZE = 0x0201,
  /// Notification that a weapon synthesis had been performed.
  PACKET_SYNTHESIZED = 0x0202,
  /// Response to the request to modify an equipment item slot.
  PACKET_EQUIPMENT_MODIFY = 0x0204,
  /// Response containing info about the materials container.
  PACKET_MATERIAL_BOX = 0x0206,
  /// Notification that the current characater's material container has been
  /// updated.
  PACKET_MATERIAL_BOX_UPDATED = 0x0207,
  /// Message containing another player character's current equipment for
  /// "analyze".
  PACKET_EQUIPMENT_ANALYZE = 0x020A,
  /// Notifies the client that another character's equipment has changed.
  PACKET_OTHER_CHARACTER_EQUIPMENT_CHANGED = 0x020B,
  /// Response to the request to extract materials from the container.
  PACKET_MATERIAL_EXTRACT = 0x020D,
  /// Response to the request to insert materials into the container.
  PACKET_MATERIAL_INSERT = 0x020F,
  /// Response to the request to exchange an item for something else.
  PACKET_ITEM_EXCHANGE = 0x0211,
  /// Response to the request to list all the available COMP shops.
  PACKET_COMP_SHOP_LIST = 0x0215,
  /// Request to the client to signify a special direction to the player.
  PACKET_EVENT_SPECIAL_DIRECTION = 0x0216,
  /// Response containing the player's fusion gauge state.
  PACKET_FUSION_GAUGE = 0x0218,
  /// Notification that the player has received a partner demon present.
  PACKET_DEMON_PRESENT = 0x0219,
  /// Notification of server supplied random numbers for an entity. Used by
  /// skills.
  PACKET_RANDOM_NUMBERS = 0x021A,
  /// Response containing the list of available titles.
  PACKET_TITLE_LIST = 0x021C,
  /// Response to the request to update the the character's active title.
  PACKET_TITLE_ACTIVE_UPDATE = 0x021E,
  /// Notification that a character's active title has been updated.
  PACKET_TITLE_ACTIVE = 0x021F,
  /// Response to the request to build a new character title.
  PACKET_TITLE_BUILD = 0x0221,
  /// Notification that the character's title list has been updated.
  PACKET_TITLE_LIST_UPDATED = 0x0222,
  /// Notification that demon quests have been made available.
  PACKET_DEMON_QUEST_LIST_UPDATED = 0x0223,
  /// Response to the request to retrieve a pending demon quest from the COMP.
  PACKET_DEMON_QUEST_DATA = 0x0225,
  /// Response to the request to accept a pending demon quest.
  PACKET_DEMON_QUEST_ACCEPT = 0x0227,
  /// Notification that the active demon quest's target counts have updated.
  PACKET_DEMON_QUEST_COUNT_UPDATE = 0x0228,
  /// Notification that the current demon quest has ended in success for
  /// failure.
  PACKET_DEMON_QUEST_END = 0x022A,
  /// Response to the request to cancel the active demon quest.
  PACKET_DEMON_QUEST_CANCEL = 0x022C,
  /// Response containing the player's partner demon quest list.
  PACKET_DEMON_QUEST_LIST = 0x022E,
  /// Response to the request for information about the active demon quest.
  PACKET_DEMON_QUEST_ACTIVE = 0x0230,
  /// Notification that the client has received items in their post from some
  /// source.
  PACKET_ITEM_DISTRIBUTION = 0x0231,
  /// Response to lock a demon in the COMP.
  PACKET_DEMON_LOCK = 0x0234,
  /// Response to the request to reunion the summoned partner demon.
  PACKET_DEMON_REUNION = 0x0236,
  /// Request to display a "congratulations" message from the partner demon
  /// during levelup.
  PACKET_LEVEL_UP_CONGRATS = 0x0238,
  /// Notifies the client that a partner demon has leveled down.
  PACKET_PARTNER_LEVEL_DOWN = 0x0239,
  /// Response to the request to reject a pending demon quest.
  PACKET_DEMON_QUEST_REJECT = 0x023B,
  /// Notification that a player entity has opened a random item box.
  PACKET_RANDOM_BOX = 0x023C,
  /// Notification that the player has been matched in PvP and is requesting
  /// confirmation.
  PACKET_PVP_READY = 0x023D,
  /// Response to the request to confirm a PvP match queue invitation.
  PACKET_PVP_CONFIRM = 0x023F,
  /// Message containing PvP match data for the current instance with all
  /// current players.
  PACKET_PVP_START = 0x0240,
  /// Mesage containing data about a PvP base in the zone.
  PACKET_PVP_BASE_DATA = 0x0241,
  /// Response to the request to start capturing a PvP base.
  PACKET_PVP_BASE_CAPTURE = 0x0243,
  /// Notification that a PvP base is currently being captured.
  PACKET_PVP_BASE_CAPTURED = 0x0244,
  /// Notification that the PvP match points have been updated.
  PACKET_PVP_POINTS = 0x0245,
  /// Message containing the results of a complete PvP match.
  PACKET_PVP_RESULT = 0x0246,
  /// Notification that a player has stopped capturing a PvP base.
  PACKET_PVP_BASE_LEFT = 0x0248,
  /// Response to the request of a solo player or team lead to enter the PvP
  /// queue.
  PACKET_PVP_JOIN = 0x024A,
  /// Response to the request of a solo player or team lead to leave the PvP
  /// queue.
  PACKET_PVP_CANCEL = 0x024C,
  /// Response containing PvP character information.
  PACKET_PVP_CHARACTER_INFO = 0x024E,
  /// Unused. Response to the request for some sort of PvP world information.
  PACKET_PVP_WORLD = 0x0250,
  /// Notification that the client will be moved from their channel to another.
  PACKET_CHANNEL_CHANGED = 0x0251,
  /// Notification that a player has either entered or left the PvP match.
  PACKET_PVP_PLAYER = 0x0252,
  /// Notification that the player's current or total BP amount has been
  /// updated.
  PACKET_BP_UPDATE = 0x0258,
  /// Notification that the current PvP queue entry count has changed.
  PACKET_PVP_ENTRY_COUNT = 0x0259,
  /// Response to the request to update the character's auto-recovery settings.
  PACKET_AUTO_RECOVERY_UPDATE = 0x025B,
  /// Message containing the current character's auto-recovery settings.
  PACKET_AUTO_RECOVERY = 0x025C,
  /// Response to the request to mix two items into a different result item.
  PACKET_ITEM_MIX = 0x025E,
  /// Notification that two items have been mixed into a different result item.
  PACKET_ITEM_MIXED = 0x025F,
  /// Response to the request to start boosting on a bike.
  PACKET_BIKE_BOOST_ON = 0x0261,
  /// Response to the request to stop boosting on a bike.
  PACKET_BIKE_BOOST_OFF = 0x0263,
  /// Response to the request to dismount from a bike.
  PACKET_BIKE_DISMOUNT = 0x0265,
  /// Response to the request to form a new team.
  PACKET_TEAM_FORM = 0x0268,
  /// Response to the request to invite a player to join the player's team.
  PACKET_TEAM_INVITE = 0x026A,
  /// Notification that the player has been invited to join a team.
  PACKET_TEAM_INVITED = 0x026B,
  /// Response to the request to either accept or reject a team invite.
  PACKET_TEAM_ANSWER = 0x026D,
  /// Notification that a player has either accepted or rejected a team invite.
  PACKET_TEAM_ANSWERED = 0x026E,
  /// Notification that a player has been added to the current team.
  PACKET_TEAM_MEMBER_ADD = 0x026F,
  /// Response to the request to kick a player from the player's team.
  PACKET_TEAM_KICK = 0x0271,
  /// Notification that the player has been kicked from their team.
  PACKET_TEAM_KICKED = 0x0272,
  /// Response to the request to update the player's current team leader.
  PACKET_TEAM_LEADER_UPDATE = 0x0274,
  /// Notification that the player's current team leader has changed.
  PACKET_TEAM_LEADER_UPDATED = 0x0275,
  /// Response to the request to have the player leave their current team.
  PACKET_TEAM_LEAVE = 0x0277,
  /// Notification that a player has left the current team.
  PACKET_TEAM_LEFT = 0x0278,
  /// Chat message sent from a member of the same team.
  PACKET_TEAM_CHAT = 0x027A,
  /// Response containing the current player's team information.
  PACKET_TEAM_INFO = 0x027C,
  /// Response to the request to list all members in the player's team.
  PACKET_TEAM_MEMBER_LIST = 0x027E,
  /// Unused. Response to the request to update some type of player team info.
  PACKET_TEAM_INFO_UPDATE = 0x0280,
  /// Unused. Notification that a player's team info has been updated.
  PACKET_TEAM_INFO_UPDATED = 0x0281,
  /// Notification that the player's current team has entered a PvP queue.
  PACKET_PVP_TEAM_JOIN = 0x0283,
  /// Notification that the player's current team has left a PvP queue.
  PACKET_PVP_TEAM_CANCEL = 0x0284,
  /// Notification that the client character has gained XP from a special item.
  PACKET_ITEM_XP = 0x0286,
  /// Response to the request to perform equipment spirit fusion.
  PACKET_EQUIPMENT_SPIRIT_FUSE = 0x0288,
  /// Notification that equipment spirit fusion was performed.
  PACKET_EQUIPMENT_SPIRIT_FUSED = 0x028B,
  /// Request to the client to start a multitalk event.
  PACKET_EVENT_MULTITALK = 0x028D,
  /// Response to the request to re-send a pending demon quest.
  PACKET_DEMON_QUEST_PENDING = 0x0290,
  /// Request to the client to inform the player that a items have been
  /// obtained.
  PACKET_EVENT_GET_ITEMS = 0x0291,
  /// Unused. MIscellaneous operation error handler.
  PACKET_ERROR_MISC = 0x0293,
  /// Request to the client to play background music as part of an event.
  PACKET_EVENT_PLAY_BGM = 0x0294,
  /// Request to the client to stop playing specific background music as part of
  /// an event.
  PACKET_EVENT_STOP_BGM = 0x0295,
  /// Response to the request to open the remote item depos.
  PACKET_ITEM_DEPO_REMOTE = 0x0297,
  /// Notification that the client partner demon's soul points have updated.
  PACKET_SOUL_POINT_UPDATE = 0x029B,
  /// Notification that the client is in a demon only zone istance.
  PACKET_DEMON_SOLO_UPDATE = 0x029C,
  /// Notification that the client's demon only challenge has ended.
  PACKET_DEMON_SOLO_END = 0x029D,
  /// Notification containing a player in a demon only dungeon's death time-out.
  PACKET_DEMON_SOLO_DEATH_TIME = 0x029E,
  /// Message containing current Diaspora instance time info.
  PACKET_DIASPORA_TIME = 0x02BC,
  /// Notification that the current Diaspora instance has ended.
  PACKET_DIASPORA_END = 0x02BD,
  /// Message containing the current Diaspora match phase.
  PACKET_DIASPORA_PHASE = 0x02BE,
  /// Message containing all Diaspora member information.
  PACKET_DIASPORA_MEMBER = 0x02BF,
  /// Message containing information about the current Diaspora match state.
  PACKET_DIASPORA_STATUS = 0x02C0,
  /// Message containing Diaspora base information.
  PACKET_DIASPORA_BASE_DATA = 0x02C1,
  /// Message containing a list of all bases in a Diaspora instance.
  PACKET_DIASPORA_BASE_INFO = 0x02C2,
  /// Notification that a Diaspora base has been captured or reset.
  PACKET_DIASPORA_BASE_STATUS = 0x02C3,
  /// Response to the request to capture a Diaspora instance base.
  PACKET_DIASPORA_BASE_CAPTURE = 0x02C5,
  /// Notification that a Diaspora team is ready to enter the instance.
  PACKET_DIASPORA_TEAM_READY = 0x02C6,
  /// Response to the request to enter a Diaspora instance after team
  /// establishment.
  PACKET_DIASPORA_ENTER = 0x02C8,
  /// Notification that a Diaspora Quake skill is being used.
  PACKET_DIASPORA_QUAKE = 0x02CB,
  /// Notification that the player's current team has been disbanded.
  PACKET_TEAM_DISBAND = 0x02EE,
  /// Response to the request to open the remote demon depos.
  PACKET_DEMON_DEPO_REMOTE = 0x02F0,
  /// Notification of the client character's extertise extension count.
  PACKET_EXPERTISE_EXTENSION = 0x02F1,
  /// Response to request to update character common switches.
  PACKET_COMMON_SWITCH_UPDATE = 0x02F3,
  /// Response to the request for character common switch settings.
  PACKET_COMMON_SWITCH_INFO = 0x02F5,
  /// Response to the request to consume a demon force item.
  PACKET_DEMON_FORCE = 0x02F7,
  /// Response to the request to add or remove the pending force stack effect.
  PACKET_DEMON_FORCE_STACK = 0x02F9,
  /// Message containing the current character's casino coin total.
  PACKET_CASINO_COIN_TOTAL = 0x02FB,
  /// Message containing the result of a solo tri-fusion.
  PACKET_TRIFUSION_SOLO = 0x0385,
  /// Response to the request to reverse equipment spirit fusion.
  PACKET_EQUIPMENT_SPIRIT_DEFUSE = 0x0387,
  /// Notification that the demon force benefit gauge has been filled.
  PACKET_DEMON_FORCE_GAUGE = 0x0388,
  /// Request to close the demon force menu.
  PACKET_DEMON_FORCE_END = 0x038A,
  /// Message containing the current UB phase.
  PACKET_UB_PHASE = 0x038B,
  /// Message containing the current state of an UB match.
  PACKET_UB_STATE = 0x038C,
  /// Message containing all current UB participants.
  PACKET_UB_MEMBERS = 0x038D,
  /// Message containing details about UB parcipants' states.
  PACKET_UB_MEMBER_STATE = 0x038E,
  /// Response to the request to spectate a target character in a UB match.
  PACKET_UB_SPECTATE_PLAYER = 0x0390,
  /// Notification containing the result of an UB round.
  PACKET_UB_RESULT = 0x0391,
  /// Message containing the current world and character UB rankings.
  PACKET_UB_RANKING = 0x0393,
  /// Response to the request to leave a UB instance prematurely.
  PACKET_UB_LEAVE = 0x0395,
  /// Notification that the current zone's UB lotto has updated.
  PACKET_UB_LOTTO_UPDATE = 0x0396,
  /// Notification that the player has entered a UB lotto queue.
  PACKET_UB_LOTTO_STATUS = 0x0397,
  /// Notification that the player has either been accepted or rejected from a
  /// UB lotto.
  PACKET_UB_LOTTO_RESULT = 0x0399,
  /// Response to the request to create a UB lotto queue entry.
  PACKET_UB_LOTTO_JOIN = 0x039B,
  /// Notification that UB lotto recruiting will start soon.
  PACKET_UB_RECRUIT = 0x039C,
  /// Notification that UB lotto recruiting has started.
  PACKET_UB_RECRUIT_START = 0x039D,
  /// Response containing the current player's search entries.
  PACKET_SEARCH_ENTRY_INFO = 0x03A4,
  /// Response containing the current player's I-Time data.
  PACKET_ITIME_DATA = 0x03A6,
  /// Notification that an I-Time NPC's point value has updated.
  PACKET_ITIME_UPDATE = 0x03A7,
  /// Request to display an I-Time event to the client.
  PACKET_ITIME_TALK = 0x03A8,
  /// Message containing data about a culture machine in the zone.
  PACKET_CULTURE_MACHINE_DATA = 0x03AA,
  /// Notification that a culture machine in the zone has been rented or
  /// completed.
  PACKET_CULTURE_MACHINE_UPDATE = 0x03AB,
  /// Response to the request for the current player's item culture data.
  PACKET_CULTURE_DATA = 0x03AD,
  /// Response to the request to access a culture machine being interacted with.
  PACKET_CULTURE_MACHINE_ACCESS = 0x03AF,
  /// Response to the request to start using a culture machine.
  PACKET_CULTURE_START = 0x03B1,
  /// Response to the request to add an item to increase culture machine
  /// success.
  PACKET_CULTURE_ITEM = 0x03B3,
  /// Notification that the current player's culture item is done.
  PACKET_CULTURE_COMPLETE = 0x03B4,
  /// Message containing information about an culture item being picked up.
  PACKET_CULTURE_RESULT = 0x03B5,
  /// Response to the request to end interaction with a culture machine.
  PACKET_CULTURE_END = 0x03B7,
  /// Unused. Mysterious redundant notification that the player has certain
  /// skills available.
  PACKET_SKILL_STATE = 0x03B8,
  /// Response to the request to edit a previously applied equipment
  /// modification.
  PACKET_EQUIPMENT_MOD_EDIT = 0x03CC,
  /// Notification containing punitive attribute system time information. Used
  /// by Invoke.
  PACKET_PATTRIBUTE = 0x03E9,
  /// Response to the request for the next punitive attribute deadline.
  PACKET_PATTRIBUTE_DEADLINE = 0x03EB,
  /// Message containing information about the current instance's Mission state.
  PACKET_MISSION_STATE = 0x03EC,
  /// Response to the request to leave a Mission instance.
  PACKET_MISSION_LEAVE = 0x03EE,
  /// Unknown. Response to the request to end some type of match (possibly
  /// depracated).
  PACKET_GAME_OVER = 0x03F0,
  /// Response to the request to perform a mitama reunion reinforcement.
  PACKET_MITAMA_REUNION = 0x03F2,
  /// Response to the request to reset a mitama reunion growth path.
  PACKET_MITAMA_RESET = 0x03F4,
  /// Response to the request to open the demon depo.
  PACKET_DEMON_DEPO_LIST = 0x03F6,
  /// Response to the request to change demon equipment.
  PACKET_DEMON_EQUIP = 0x03FC,
  /// Unused. Notifies the client of a parter demon's calculated stats (see
  /// 0x004C).
  PACKET_DEMON_STATS = 0x03FD,
  /// Response containing the results of an NPC barter.
  PACKET_BARTER = 0x03FF,
  /// Response to the request for the current world and character Pentalpha
  /// data.
  PACKET_PENTALPHA_DATA = 0x0401,
  /// Notification that the current Pentalpha match has ended.
  PACKET_PENTALPHA_END = 0x0402,
  /// Message containing the client character's cowrie and bethel values.
  PACKET_COWRIE_BETHEL = 0x0403,
  /// Notification that the player character's available skill points have
  /// updated.
  PACKET_SKILL_POINT_UPDATE = 0x0404,
  /// Notification of the client logic adjusting world bonuses.
  PACKET_WORLD_BONUS = 0x0405,
  /// Response containing the current player's blacklist.
  PACKET_BLACKLIST = 0x0409,
  /// Response to the request to update the current player's blacklist.
  PACKET_BLACKLIST_UPDATE = 0x040B,
  /// Notification that the player has a Destiny box available.
  PACKET_DESTINY_BOX = 0x040C,
  /// Response to the request for Destiny box data.
  PACKET_DESTINY_BOX_DATA = 0x040E,
  /// Notifiation that the player's Destiny box has updated.
  PACKET_DESTINY_BOX_UPDATE = 0x040F,
  /// Response to the request to start a Destiny box item lotto.
  PACKET_DESTINY_LOTTO = 0x0411,
  /// Notification that the player character has activated digitalization.
  PACKET_DIGITALIZE_START = 0x0412,
  /// Notification that the player character has completed digitalization.
  PACKET_DIGITALIZE_END = 0x0413,
  /// Response containing the current player's digitalize point information.
  PACKET_DIGITALIZE_POINTS = 0x0415,
  /// Notification that the player's digitalize points have updated.
  PACKET_DIGITALIZE_POINTS_UPDATE = 0x0416,
  /// Notification that a player's digitalize level has increased.
  PACKET_DIGITALIZE_LEVEL_UP = 0x0417,
  /// Response containing the current player's digitalize assist information.
  PACKET_DIGITALIZE_ASSIST = 0x0419,
  /// Response to the request to learn a digitalize assist skill.
  PACKET_DIGITALIZE_ASSIST_LEARN = 0x041B,
  /// Response to the request to remove an active digitalize assist skill.
  PACKET_DIGITALIZE_ASSIST_REMOVE = 0x041D,
  /// Response to the request to list all VA items in the VA closet.
  PACKET_VA_BOX = 0x041F,
  /// Response to the request to create a VA item and add it to the closet.
  PACKET_VA_BOX_ADD = 0x0421,
  /// Response to the request to remove a VA item from the closet.
  PACKET_VA_BOX_REMOVE = 0x0423,
  /// Reponse to the request to change the current character's VA.
  PACKET_VA_CHANGE = 0x0425,
  /// Notification that a character's VA has changed.
  PACKET_VA_CHANGED = 0x0426,
  /// Response to the request to move a VA item in the closet.
  PACKET_VA_BOX_MOVE = 0x0428,
  /// Notification that the client has entered a digitalize instance.
  PACKET_DIGITALIZE_DUNGEON_START = 0x0429,
  /// Message containing the current digitalize instance state.
  PACKET_DIGITALIZE_DUNGEON_UPDATE = 0x042A,
  /// Notification that the current digitalize instance has been completed.
  PACKET_DIGITALIZE_DUNGEON_END = 0x042B,
  /// Notification that the client's team's ziotite values have updated.
  PACKET_ZIOTITE_UPDATE = 0x042C,
  /// Message containing team member position and HP info in tracked zones.
  PACKET_TEAM_MEMBER_UPDATE = 0x042D,
  /// Message containing multi-zone boss state information.
  PACKET_MULTIZONE_BOSS_STATUS = 0x042E,
  /// Notification that a multi-zone boss has been killed.
  PACKET_MULTIZONE_BOSS_KILLED = 0x042F,
  /// Message containing data about an ally NPC in the zone.
  PACKET_ALLY_DATA = 0x0431,
  /// Response to the request to retrieve the player's reunion conversion
  /// points.
  PACKET_REUNION_POINTS = 0x0433,
  /// Request to extract reunion conversion points from a demon.
  PACKET_REUNION_EXTRACT = 0x0435,
  /// Request to inject reunion conversion points into a demon.
  PACKET_REUNION_INJECT = 0x0437,

  //
  // Custom Packets (Amala Network)
  //
  /// Server version and user level
  PACKET_AMALA_SERVER_VERSION = 0x1000,
  /// Header info about the account dump
  PACKET_AMALA_ACCOUNT_DUMP_HEADER = 0x1001,
  /// Part of the account dump data
  PACKET_AMALA_ACCOUNT_DUMP_PART = 0x1002,
};

/**
 * Client specified set of flags added to the log out packet to signify the
 * expected action to take.
 */
enum class LogoutPacketAction_t : uint32_t {
  /// Prepare to log out, printing a 10 second countdown if not sent a different
  /// code shortly after.
  LOGOUT_PREPARE = 10,
  /// Logout cancel confirmation.
  LOGOUT_CANCEL = 11,
  /// Close the connection resulting in a generic disconnect message if sent not
  /// following a prepare.
  LOGOUT_DISCONNECT = 13,
  /// Log out of the current channel and move to the one specified in the
  /// packet.
  LOGOUT_CHANNEL_SWITCH = 14,
};

/**
 * Chat type sent from and returned to the client to signify what type of chat
 * message is being sent.
 */
enum ChatType_t : uint16_t {
  /// Sent via the party channel to party members, by default to all zones on
  /// the world.
  CHAT_PARTY = 41,
  /// Sent via the say/shout channel, detached from a speech bubble, by default
  /// to the current zone.
  CHAT_SHOUT = 44,
  /// Sent via the say/shout channel, displayed in a speech bubble to players
  /// near the source.
  CHAT_SAY = 45,
  /// Sent via the tell channel, targets a specific character in any zone on the
  /// world.
  CHAT_TELL = 46,
  /// Sent to one character from the system itself. Useful for communication
  /// game information.
  CHAT_SELF = 47,
  /// Sent via the clan channel to online clan members, by default to all zones
  /// on the world.
  CHAT_CLAN = 48,
  /// Sent via the versus channel to PvP team members, by default to the same
  /// zone only.
  CHAT_VERSUS = 597,
  /// Sent via the team channel to team members, by default to all zones on the
  /// world.
  CHAT_TEAM = 714,
};

/**
 * Request or response packet code between two internal servers.
 */
enum class InternalPacketCode_t : uint16_t {
  /// Request to detail the world server.
  PACKET_GET_WORLD_INFO = 0x1001,
  /// Request to update a non-world server's world information.
  PACKET_SET_WORLD_INFO = 0x1002,
  /// Request to update a server's channel information.
  PACKET_SET_CHANNEL_INFO = 0x1003,
  /// Pass login information between the servers.
  PACKET_ACCOUNT_LOGIN = 0x1004,
  /// Pass logout information between the servers.
  PACKET_ACCOUNT_LOGOUT = 0x1005,
  /// Relay a channel to client message from one player to another between
  /// servers.
  PACKET_RELAY = 0x1006,
  /// Sync records between the servers.
  PACKET_DATA_SYNC = 0x1007,
  /// Pass character login information between the servers.
  PACKET_CHARACTER_LOGIN = 0x1008,
  /// Pass friend information between the servers.
  PACKET_FRIENDS_UPDATE = 0x1009,
  /// Pass party information between the servers.
  PACKET_PARTY_UPDATE = 0x100A,
  /// Pass clan information between the servers.
  PACKET_CLAN_UPDATE = 0x100B,
  /// Pass web game information between the servers.
  PACKET_WEB_GAME = 0x100C,
  /// Pass team information between the servers.
  PACKET_TEAM_UPDATE = 0x100D,
};

/**
 * Mode signifier that changes how a packet should be handled.
 */
enum class InternalPacketAction_t : uint8_t {
  /// Indicates a contextual addition.
  PACKET_ACTION_ADD = 1,
  /// Indicates a contextual update.
  PACKET_ACTION_UPDATE,
  /// Indicates a contextual removal.
  PACKET_ACTION_REMOVE,
  /// Indicates a contextual Yes/No request.
  PACKET_ACTION_YN_REQUEST,
  /// Indicates a contextual "Yes" response.
  PACKET_ACTION_RESPONSE_YES,
  /// Indicates a contextual "No" response.
  PACKET_ACTION_RESPONSE_NO,

  /// Indicates that a contextual group list is being requested or sent.
  PACKET_ACTION_GROUP_LIST,
  /// Indicates that a contextual group update consists of "leave" information.
  PACKET_ACTION_GROUP_LEAVE,
  /// Indicates that a contextual group update consists of "disband"
  /// information.
  PACKET_ACTION_GROUP_DISBAND,
  /// Indicates that a contextual group update consists of "leader update"
  /// information.
  PACKET_ACTION_GROUP_LEADER_UPDATE,
  /// Indicates that a contextual group update consists of "kick" information.
  PACKET_ACTION_GROUP_KICK,

  /// Indicates that a clan update consists of "emblem update" information.
  PACKET_ACTION_CLAN_EMBLEM_UPDATE,
  /// Indicates that a party update consists of "drop rule" information.
  PACKET_ACTION_PARTY_DROP_RULE,
  /// Indicates that a team update consists of an updated ziotite gauge.
  PACKET_ACTION_TEAM_ZIOTITE,
};

/**
 * Specifies how the message should be relayed. All options are valid when
 * communicating with the world server and only the failure or world CIDs
 * options are used when the world relays the packet to a channel.
 */
enum class PacketRelayMode_t : uint8_t {
  /// Response back to the world containing CIDs that were not in the channel to
  /// attempt a retry.
  RELAY_FAILURE = 0,
  /// Requst to relay a message to a account by UUID.
  RELAY_ACCOUNT,
  /// Request to relay a message to a character by name.
  RELAY_CHARACTER,
  /// Request to relay a message to one or more characters by world CID.
  RELAY_CIDS,
  /// Request to relay a message to all current members of a party.
  RELAY_PARTY,
  /// Request to relay a message to all online clan members.
  RELAY_CLAN,
  /// Request to relay a message to all current team members.
  RELAY_TEAM,
  /// Request to relay a message to everyone on every channel connected to the
  /// world.
  RELAY_ALL,
};

/**
 * Flags used to indicate the type of information being sent in a
 * PACKET_CHARACTER_LOGIN message.  Data signified to have been added to the
 * packet should be written from lowest to highest value listed here.
 */
enum class CharacterLoginStateFlag_t : uint8_t {
  /// Indicates that a player's active status is contained in a packet.
  CHARLOGIN_STATUS = 0x01,
  /// Indicates that a character's zone is contained in a packet.
  CHARLOGIN_ZONE = 0x02,
  /// Indicates that a character's channel is contained in a packet.
  CHARLOGIN_CHANNEL = 0x04,
  /// Indicates that basic information about a character is contained in a
  /// packet.
  CHARLOGIN_BASIC = 0x07,
  /// Indicates that a character's friend or clan message is contained in a
  /// packet.
  CHARLOGIN_MESSAGE = 0x08,
  /// Unknown. Indicates that some friend information is being sent.
  CHARLOGIN_FRIEND_UNKNOWN = 0x10,
  /// Indicates that one or many friend related pieces of data are contained in
  /// a packet.
  CHARLOGIN_FRIEND_FLAGS = 0x1F,
  /// Indicates that player character party information is contained in a
  /// packet.
  CHARLOGIN_PARTY_INFO = 0x20,
  /// Indicates that partner demon party information is contained in a packet.
  CHARLOGIN_PARTY_DEMON_INFO = 0x40,
  /// Indicates that party icon information is contained in a packet.
  CHARLOGIN_PARTY_ICON = 0x80,
  /// Indicates that one or many party related pieces of data are contained in a
  /// packet.
  CHARLOGIN_PARTY_FLAGS = 0xE2,
};

#endif  // LIBCOMP_SRC_PACKETCODES_H
