/**
 * @file libcomp/src/ServerConstants.cpp
 * @ingroup libcomp
 *
 * @author HACKfrost
 *
 * @brief Server side configurable constants for logical concepts
 * that match binary file IDs.
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

#include "ServerConstants.h"

// Standard C++ Includes
#include <unordered_map>

// Ignore warnings
#include <PushIgnore.h>

// tinyxml2 Includes
#include <tinyxml2.h>

// Stop ignoring warnings
#include <PopIgnore.h>

// libcomp Includes
#include <Log.h>

using namespace libcomp;

ServerConstants::Data ServerConstants::sConstants = {};

bool ServerConstants::Initialize(const String& filePath) {
  tinyxml2::XMLDocument doc;
  if (tinyxml2::XML_SUCCESS != doc.LoadFile(filePath.C())) {
    LogServerConstantsErrorMsg("Server contants XML is not valid.");
    return false;
  }

  // Read all constants as strings
  std::unordered_map<std::string, std::string> constants;
  std::unordered_map<std::string, const tinyxml2::XMLElement*> complexConstants;
  const tinyxml2::XMLElement* pMember =
      doc.RootElement()->FirstChildElement("constant");
  while (pMember) {
    const char* szName = pMember->Attribute("name");

    if (szName && pMember->FirstChild()) {
      const tinyxml2::XMLElement* child = pMember->FirstChildElement();
      if (child) {
        complexConstants[szName] = child;
      } else {
        const tinyxml2::XMLText* pText = pMember->FirstChild()->ToText();
        if (pText) {
          constants[szName] = pText->Value();
        }
      }
    }

    pMember = pMember->NextSiblingElement("constant");
  }

  // Convert and assign all entries
  bool success = true;

  // Load demon constants
  success &= LoadInteger(constants["ELEMENTAL_1_FLAEMIS"],
                         sConstants.ELEMENTAL_1_FLAEMIS);
  success &= LoadInteger(constants["ELEMENTAL_2_AQUANS"],
                         sConstants.ELEMENTAL_2_AQUANS);
  success &=
      LoadInteger(constants["ELEMENTAL_3_AEROS"], sConstants.ELEMENTAL_3_AEROS);
  success &= LoadInteger(constants["ELEMENTAL_4_ERTHYS"],
                         sConstants.ELEMENTAL_4_ERTHYS);
  success &= LoadInteger(constants["MITAMA_1_ARAMITAMA"],
                         sConstants.MITAMA_1_ARAMITAMA);
  success &= LoadInteger(constants["MITAMA_2_NIGIMITAMA"],
                         sConstants.MITAMA_2_NIGIMITAMA);
  success &= LoadInteger(constants["MITAMA_3_KUSHIMITAMA"],
                         sConstants.MITAMA_3_KUSHIMITAMA);
  success &= LoadInteger(constants["MITAMA_4_SAKIMITAMA"],
                         sConstants.MITAMA_4_SAKIMITAMA);

  // Load item constants
  success &= LoadInteger(constants["ITEM_MACCA"], sConstants.ITEM_MACCA);
  success &=
      LoadInteger(constants["ITEM_MACCA_NOTE"], sConstants.ITEM_MACCA_NOTE);
  success &=
      LoadInteger(constants["ITEM_MAGNETITE"], sConstants.ITEM_MAGNETITE);
  success &=
      LoadInteger(constants["ITEM_MAG_PRESSER"], sConstants.ITEM_MAG_PRESSER);
  success &=
      LoadInteger(constants["ITEM_BALM_OF_LIFE"], sConstants.ITEM_BALM_OF_LIFE);
  success &= LoadInteger(constants["ITEM_BALM_OF_LIFE_DEMON"],
                         sConstants.ITEM_BALM_OF_LIFE_DEMON);
  success &= LoadInteger(constants["ITEM_KREUZ"], sConstants.ITEM_KREUZ);
  success &=
      LoadInteger(constants["ITEM_RBLOODSTONE"], sConstants.ITEM_RBLOODSTONE);

  // Load menu constants
  success &= LoadInteger(constants["MENU_BARTER"], sConstants.MENU_BARTER);
  success &= LoadInteger(constants["MENU_BAZAAR"], sConstants.MENU_BAZAAR);
  success &=
      LoadInteger(constants["MENU_COMP_SHOP"], sConstants.MENU_COMP_SHOP);
  success &= LoadInteger(constants["MENU_CULTURE"], sConstants.MENU_CULTURE);
  success &=
      LoadInteger(constants["MENU_DEMON_DEPO"], sConstants.MENU_DEMON_DEPO);
  success &=
      LoadInteger(constants["MENU_FUSION_KZ"], sConstants.MENU_FUSION_KZ);
  success &=
      LoadInteger(constants["MENU_ITEM_DEPO"], sConstants.MENU_ITEM_DEPO);
  success &= LoadInteger(constants["MENU_ITIME"], sConstants.MENU_ITIME);
  success &=
      LoadInteger(constants["MENU_REPAIR_KZ"], sConstants.MENU_REPAIR_KZ);
  success &= LoadInteger(constants["MENU_SHOP_BUY"], sConstants.MENU_SHOP_BUY);
  success &=
      LoadInteger(constants["MENU_SHOP_REPAIR"], sConstants.MENU_SHOP_REPAIR);
  success &=
      LoadInteger(constants["MENU_SHOP_SELL"], sConstants.MENU_SHOP_SELL);
  success &=
      LoadInteger(constants["MENU_TRIFUSION"], sConstants.MENU_TRIFUSION);
  success &=
      LoadInteger(constants["MENU_TRIFUSION_KZ"], sConstants.MENU_TRIFUSION_KZ);
  success &=
      LoadInteger(constants["MENU_UB_RANKING"], sConstants.MENU_UB_RANKING);
  success &= LoadInteger(constants["MENU_WEB_GAME"], sConstants.MENU_WEB_GAME);

  // Load skill constants
  success &=
      LoadInteger(constants["SKILL_ABS_DAMAGE"], sConstants.SKILL_ABS_DAMAGE);
  success &= LoadInteger(constants["SKILL_CAMEO"], sConstants.SKILL_CAMEO);
  success &=
      LoadInteger(constants["SKILL_CLAN_FORM"], sConstants.SKILL_CLAN_FORM);
  success &= LoadInteger(constants["SKILL_CLOAK"], sConstants.SKILL_CLOAK);
  success &= LoadInteger(constants["SKILL_GENDER_RESTRICTED"],
                         sConstants.SKILL_GENDER_RESTRICTED);
  success &= LoadInteger(constants["SKILL_CULTURE_SLOT_UP"],
                         sConstants.SKILL_CULTURE_SLOT_UP);
  success &=
      LoadInteger(constants["SKILL_CULTURE_UP"], sConstants.SKILL_CULTURE_UP);
  success &= LoadInteger(constants["SKILL_DCM"], sConstants.SKILL_DCM);
  success &= LoadInteger(constants["SKILL_DEMON_FUSION"],
                         sConstants.SKILL_DEMON_FUSION);
  success &= LoadInteger(constants["SKILL_DEMON_FUSION_EXECUTE"],
                         sConstants.SKILL_DEMON_FUSION_EXECUTE);
  success &= LoadInteger(constants["SKILL_DESPAWN"], sConstants.SKILL_DESPAWN);
  success &=
      LoadInteger(constants["SKILL_DESUMMON"], sConstants.SKILL_DESUMMON);
  success &= LoadInteger(constants["SKILL_DIASPORA_QUAKE"],
                         sConstants.SKILL_DIASPORA_QUAKE);
  success &=
      LoadInteger(constants["SKILL_DIGITALIZE"], sConstants.SKILL_DIGITALIZE);
  success &= LoadInteger(constants["SKILL_DIGITALIZE_BREAK"],
                         sConstants.SKILL_DIGITALIZE_BREAK);
  success &= LoadInteger(constants["SKILL_DIGITALIZE_CANCEL"],
                         sConstants.SKILL_DIGITALIZE_CANCEL);
  success &= LoadInteger(constants["SKILL_DURABILITY_DOWN"],
                         sConstants.SKILL_DURABILITY_DOWN);
  success &=
      LoadInteger(constants["SKILL_EQUIP_ITEM"], sConstants.SKILL_EQUIP_ITEM);
  success &= LoadInteger(constants["SKILL_EQUIP_MOD_EDIT"],
                         sConstants.SKILL_EQUIP_MOD_EDIT);
  success &= LoadInteger(constants["SKILL_ESTOMA"], sConstants.SKILL_ESTOMA);
  success &= LoadInteger(constants["SKILL_EXPERT_CLASS_DOWN"],
                         sConstants.SKILL_EXPERT_CLASS_DOWN);
  success &= LoadInteger(constants["SKILL_EXPERT_FORGET"],
                         sConstants.SKILL_EXPERT_FORGET);
  success &= LoadInteger(constants["SKILL_EXPERT_FORGET_ALL"],
                         sConstants.SKILL_EXPERT_FORGET_ALL);
  success &= LoadInteger(constants["SKILL_EXPERT_RANK_DOWN"],
                         sConstants.SKILL_EXPERT_RANK_DOWN);
  success &= LoadInteger(constants["SKILL_FAM_UP"], sConstants.SKILL_FAM_UP);
  success &=
      LoadInteger(constants["SKILL_GEM_COST"], sConstants.SKILL_GEM_COST);
  success &= LoadInteger(constants["SKILL_HP_DEPENDENT"],
                         sConstants.SKILL_HP_DEPENDENT);
  success &=
      LoadInteger(constants["SKILL_HP_MP_MIN"], sConstants.SKILL_HP_MP_MIN);
  success &=
      LoadInteger(constants["SKILL_ITEM_FAM_UP"], sConstants.SKILL_ITEM_FAM_UP);
  success &=
      LoadInteger(constants["SKILL_LIBERAMA"], sConstants.SKILL_LIBERAMA);
  success &=
      LoadInteger(constants["SKILL_LNC_DAMAGE"], sConstants.SKILL_LNC_DAMAGE);
  success &= LoadInteger(constants["SKILL_MAX_DURABILITY_FIXED"],
                         sConstants.SKILL_MAX_DURABILITY_FIXED);
  success &= LoadInteger(constants["SKILL_MAX_DURABILITY_RANDOM"],
                         sConstants.SKILL_MAX_DURABILITY_RANDOM);
  success &= LoadInteger(constants["SKILL_MINION_DESPAWN"],
                         sConstants.SKILL_MINION_DESPAWN);
  success &= LoadInteger(constants["SKILL_MINION_SPAWN"],
                         sConstants.SKILL_MINION_SPAWN);
  success &= LoadInteger(constants["SKILL_MOOCH"], sConstants.SKILL_MOOCH);
  success &= LoadInteger(constants["SKILL_MOUNT"], sConstants.SKILL_MOUNT);
  success &= LoadInteger(constants["SKILL_PIERCE"], sConstants.SKILL_PIERCE);
  success &=
      LoadInteger(constants["SKILL_RANDOM_ITEM"], sConstants.SKILL_RANDOM_ITEM);
  success &=
      LoadInteger(constants["SKILL_RANDOMIZE"], sConstants.SKILL_RANDOMIZE);
  success &= LoadInteger(constants["SKILL_RESPEC"], sConstants.SKILL_RESPEC);
  success &= LoadInteger(constants["SKILL_REST"], sConstants.SKILL_REST);
  success &= LoadInteger(constants["SKILL_SLEEP_RESTRICTED"],
                         sConstants.SKILL_SLEEP_RESTRICTED);
  success &= LoadInteger(constants["SKILL_SPAWN"], sConstants.SKILL_SPAWN);
  success &=
      LoadInteger(constants["SKILL_SPAWN_ZONE"], sConstants.SKILL_SPAWN_ZONE);
  success &= LoadInteger(constants["SKILL_SPECIAL_REQUEST"],
                         sConstants.SKILL_SPECIAL_REQUEST);
  success &= LoadInteger(constants["SKILL_STAT_SUM_DAMAGE"],
                         sConstants.SKILL_STAT_SUM_DAMAGE);
  success &= LoadInteger(constants["SKILL_STATUS_DIRECT"],
                         sConstants.SKILL_STATUS_DIRECT);
  success &= LoadInteger(constants["SKILL_STATUS_LIMITED"],
                         sConstants.SKILL_STATUS_LIMITED);
  success &= LoadInteger(constants["SKILL_STATUS_RANDOM"],
                         sConstants.SKILL_STATUS_RANDOM);
  success &= LoadInteger(constants["SKILL_STATUS_RANDOM2"],
                         sConstants.SKILL_STATUS_RANDOM2);
  success &= LoadInteger(constants["SKILL_STATUS_RESTRICTED"],
                         sConstants.SKILL_STATUS_RESTRICTED);
  success &= LoadInteger(constants["SKILL_STATUS_SCALE"],
                         sConstants.SKILL_STATUS_SCALE);
  success &=
      LoadInteger(constants["SKILL_STORE_DEMON"], sConstants.SKILL_STORE_DEMON);
  success &= LoadInteger(constants["SKILL_SUICIDE"], sConstants.SKILL_SUICIDE);
  success &= LoadInteger(constants["SKILL_SUMMON_DEMON"],
                         sConstants.SKILL_SUMMON_DEMON);
  success &= LoadInteger(constants["SKILL_TAUNT"], sConstants.SKILL_TAUNT);
  success &= LoadInteger(constants["SKILL_TRAESTO"], sConstants.SKILL_TRAESTO);
  success &= LoadInteger(constants["SKILL_WARP"], sConstants.SKILL_WARP);
  success &=
      LoadInteger(constants["SKILL_XP_PARTNER"], sConstants.SKILL_XP_PARTNER);
  success &= LoadInteger(constants["SKILL_XP_SELF"], sConstants.SKILL_XP_SELF);
  success &= LoadInteger(constants["SKILL_ZONE_RESTRICTED"],
                         sConstants.SKILL_ZONE_RESTRICTED);
  success &= LoadInteger(constants["SKILL_ZONE_RESTRICTED_ITEM"],
                         sConstants.SKILL_ZONE_RESTRICTED_ITEM);
  success &= LoadInteger(constants["SKILL_ZONE_TARGET_ALL"],
                         sConstants.SKILL_ZONE_TARGET_ALL);

  // Load status effect constants
  success &= LoadInteger(constants["STATUS_BIKE"], sConstants.STATUS_BIKE);
  success &= LoadInteger(constants["STATUS_CLOAK"], sConstants.STATUS_CLOAK);
  success &= LoadInteger(constants["STATUS_DEATH"], sConstants.STATUS_DEATH);
  success &=
      LoadInteger(constants["STATUS_DEMON_ONLY"], sConstants.STATUS_DEMON_ONLY);
  success &= LoadInteger(constants["STATUS_DEMON_QUEST_ACTIVE"],
                         sConstants.STATUS_DEMON_QUEST_ACTIVE);
  success &= LoadInteger(constants["STATUS_DIGITALIZE_COOLDOWN"],
                         sConstants.STATUS_DIGITALIZE_COOLDOWN);
  success &= LoadInteger(constants["STATUS_MOUNT"], sConstants.STATUS_MOUNT);
  success &= LoadInteger(constants["STATUS_MOUNT_SUPER"],
                         sConstants.STATUS_MOUNT_SUPER);
  success &= LoadInteger(constants["STATUS_REUNION_XP_SAVE"],
                         sConstants.STATUS_REUNION_XP_SAVE);
  success &= LoadInteger(constants["STATUS_SLEEP"], sConstants.STATUS_SLEEP);
  success &=
      LoadInteger(constants["STATUS_STEALTH"], sConstants.STATUS_STEALTH);
  success &= LoadInteger(constants["STATUS_SUMMON_SYNC_1"],
                         sConstants.STATUS_SUMMON_SYNC_1);
  success &= LoadInteger(constants["STATUS_SUMMON_SYNC_2"],
                         sConstants.STATUS_SUMMON_SYNC_2);
  success &= LoadInteger(constants["STATUS_SUMMON_SYNC_3"],
                         sConstants.STATUS_SUMMON_SYNC_3);

  // Load (detached) tokusei constants
  success &= LoadInteger(constants["TOKUSEI_BIKE_BOOST"],
                         sConstants.TOKUSEI_BIKE_BOOST);
  success &= LoadInteger(constants["TOKUSEI_MAGIC_CONTROL_COST"],
                         sConstants.TOKUSEI_MAGIC_CONTROL_COST);

  // Load valuable constants
  success &= LoadInteger(constants["VALUABLE_DEVIL_BOOK_V1"],
                         sConstants.VALUABLE_DEVIL_BOOK_V1);
  success &= LoadInteger(constants["VALUABLE_DEVIL_BOOK_V2"],
                         sConstants.VALUABLE_DEVIL_BOOK_V2);
  success &= LoadInteger(constants["VALUABLE_DEMON_FORCE"],
                         sConstants.VALUABLE_DEMON_FORCE);
  success &= LoadInteger(constants["VALUABLE_DIGITALIZE_LV1"],
                         sConstants.VALUABLE_DIGITALIZE_LV1);
  success &= LoadInteger(constants["VALUABLE_DIGITALIZE_LV2"],
                         sConstants.VALUABLE_DIGITALIZE_LV2);
  success &= LoadInteger(constants["VALUABLE_FUSION_GAUGE"],
                         sConstants.VALUABLE_FUSION_GAUGE);
  success &= LoadInteger(constants["VALUABLE_MATERIAL_TANK"],
                         sConstants.VALUABLE_MATERIAL_TANK);

  // Load other constants
  success &= LoadInteger(constants["DIGITALIZE_COOLDOWN"],
                         sConstants.DIGITALIZE_COOLDOWN);
  success &= LoadInteger(constants["MAX_MOVE_INCREASE_SUM"],
                         sConstants.MAX_MOVE_INCREASE_SUM);
  success &=
      LoadInteger(constants["MITAMA_SET_BOOST"], sConstants.MITAMA_SET_BOOST);
  success &= LoadInteger(constants["ZONE_DEFAULT"], sConstants.ZONE_DEFAULT);

  String listStr;
  success &=
      LoadString(constants["DIGITALIZE_STAT_RATES"], listStr) &&
      ToIntegerArray(sConstants.DIGITALIZE_STAT_RATES, listStr.Split(","));
  success &=
      LoadString(constants["SKILL_TRAESTO_ARCADIA"], listStr) &&
      ToIntegerArray(sConstants.SKILL_TRAESTO_ARCADIA, listStr.Split(","));
  success &=
      LoadString(constants["SKILL_TRAESTO_DSHINJUKU"], listStr) &&
      ToIntegerArray(sConstants.SKILL_TRAESTO_DSHINJUKU, listStr.Split(","));
  success &=
      LoadString(constants["SKILL_TRAESTO_KAKYOJO"], listStr) &&
      ToIntegerArray(sConstants.SKILL_TRAESTO_KAKYOJO, listStr.Split(","));
  success &= LoadString(constants["SKILL_TRAESTO_NAKANO_BDOMAIN"], listStr) &&
             ToIntegerArray(sConstants.SKILL_TRAESTO_NAKANO_BDOMAIN,
                            listStr.Split(","));
  success &=
      LoadString(constants["SKILL_TRAESTO_SOUHONZAN"], listStr) &&
      ToIntegerArray(sConstants.SKILL_TRAESTO_SOUHONZAN, listStr.Split(","));

  success &= LoadString(constants["STATUS_COMP_TUNING"], listStr) &&
             ToIntegerSet(sConstants.STATUS_COMP_TUNING, listStr.Split(","));
  success &= LoadString(constants["STATUS_DIGITALIZE"], listStr) &&
             ToIntegerArray(sConstants.STATUS_DIGITALIZE, listStr.Split(","));

  if (!success) {
    LogServerConstantsErrorMsg(
        "Failed to load one or more primitive constant values\n");
    return false;
  }

  auto complexIter = complexConstants.find("ADJUSTMENT_ITEMS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load ADJUSTMENT_ITEMS\n");
      return false;
    }

    for (auto pair : map) {
      uint32_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load ADJUSTMENT_ITEMS key\n");
        return false;
      } else if (sConstants.ADJUSTMENT_ITEMS.find(key) !=
                 sConstants.ADJUSTMENT_ITEMS.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate ADJUSTMENT_ITEMS key encountered\n");
        return false;
      } else if (!ToIntegerArray(sConstants.ADJUSTMENT_ITEMS[key],
                                 libcomp::String(pair.second).Split(","))) {
        LogServerConstantsErrorMsg(
            "Failed to load an element in ADJUSTMENT_ITEMS\n");
        return false;
      }
    }
  } else {
    LogServerConstantsErrorMsg("ADJUSTMENT_ITEMS not found\n");
    return false;
  }

  complexIter = complexConstants.find("ADJUSTMENT_SKILLS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load ADJUSTMENT_SKILLS\n");
      return false;
    }

    for (auto pair : map) {
      uint32_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load ADJUSTMENT_SKILLS key\n");
        return false;
      } else if (sConstants.ADJUSTMENT_SKILLS.find(key) !=
                 sConstants.ADJUSTMENT_SKILLS.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate ADJUSTMENT_SKILLS key encountered\n");
        return false;
      } else if (!ToIntegerArray(sConstants.ADJUSTMENT_SKILLS[key],
                                 libcomp::String(pair.second).Split(","))) {
        LogServerConstantsErrorMsg(
            "Failed to load an element in ADJUSTMENT_SKILLS\n");
        return false;
      }
    }
  } else {
    LogServerConstantsErrorMsg("ADJUSTMENT_SKILLS not found\n");
    return false;
  }

  complexIter = complexConstants.find("BARTER_COOLDOWNS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.BARTER_COOLDOWNS)) {
      LogServerConstantsErrorMsg("Failed to load BARTER_COOLDOWNS\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("BARTER_COOLDOWNS not found\n");
    return false;
  }

  complexIter = complexConstants.find("CAMEO_MAP");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load CAMEO_MAP\n");
      return false;
    }

    for (auto pair : map) {
      uint16_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load CAMEO_MAP key\n");
        return false;
      } else if (sConstants.CAMEO_MAP.find(key) != sConstants.CAMEO_MAP.end()) {
        LogServerConstantsErrorMsg("Duplicate CAMEO_MAP key encountered\n");
        return false;
      } else {
        if (!pair.second.empty()) {
          for (uint32_t p : ToIntegerRange<uint32_t>(pair.second, success)) {
            sConstants.CAMEO_MAP[key].push_back(p);
          }

          if (!success) {
            LogServerConstantsErrorMsg(
                "Failed to load an element in CAMEO_MAP\n");
            return false;
          }
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("CAMEO_MAP not found\n");
    return false;
  }

  complexIter = complexConstants.find("CLAN_FORM_MAP");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.CLAN_FORM_MAP)) {
      LogServerConstantsErrorMsg("Failed to load CLAN_FORM_MAP\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("CLAN_FORM_MAP not found\n");
    return false;
  }

  complexIter = complexConstants.find("CLAN_LEVEL_SKILLS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load CLAN_LEVEL_SKILLS\n");
      return false;
    } else {
      if (strList.size() != 10) {
        LogServerConstantsErrorMsg(
            "CLAN_LEVEL_SKILLS must specify skills for all 10 levels\n");
        return false;
      } else {
        size_t idx = 0;
        for (auto elemStr : strList) {
          if (!elemStr.IsEmpty()) {
            for (uint32_t p : ToIntegerRange<uint32_t>(elemStr.C(), success)) {
              sConstants.CLAN_LEVEL_SKILLS[idx].insert(p);
            }

            if (!success) {
              LogServerConstantsErrorMsg(
                  "Failed to load an element in CLAN_LEVEL_SKILLS\n");
              return false;
            }
          }

          idx++;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("CLAN_LEVEL_SKILLS not found\n");
    return false;
  }

  complexIter = complexConstants.find("DEMON_BOOK_BONUS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load DEMON_BOOK_BONUS\n");
      return false;
    }

    for (auto pair : map) {
      uint16_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load DEMON_BOOK_BONUS key\n");
        return false;
      } else if (sConstants.DEMON_BOOK_BONUS.find(key) !=
                 sConstants.DEMON_BOOK_BONUS.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate DEMON_BOOK_BONUS key encountered\n");
        return false;
      } else {
        if (!pair.second.empty()) {
          for (int32_t p : ToIntegerRange<int32_t>(pair.second, success)) {
            sConstants.DEMON_BOOK_BONUS[key].insert(p);
          }

          if (!success) {
            LogServerConstantsErrorMsg(
                "Failed to load an element in DEMON_BOOK_BONUS\n");
            return false;
          }
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("DEMON_BOOK_BONUS not found\n");
    return false;
  }

  complexIter = complexConstants.find("DEMON_CRYSTALS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load DEMON_CRYSTALS\n");
      return false;
    }

    for (auto pair : map) {
      uint32_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load DEMON_CRYSTALS key\n");
        return false;
      } else if (sConstants.DEMON_CRYSTALS.find(key) !=
                 sConstants.DEMON_CRYSTALS.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate DEMON_CRYSTALS key encountered\n");
        return false;
      } else {
        if (!pair.second.empty()) {
          for (uint8_t p : ToIntegerRange<uint8_t>(pair.second, success)) {
            sConstants.DEMON_CRYSTALS[key].insert(p);
          }

          if (!success) {
            LogServerConstantsErrorMsg(
                "Failed to load an element in DEMON_CRYSTALS\n");
            return false;
          }
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("DEMON_CRYSTALS not found\n");
    return false;
  }

  complexIter = complexConstants.find("DEMON_FUSION_SKILLS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load DEMON_FUSION_SKILLS\n");
      return false;
    } else {
      if (strList.size() != 21) {
        LogServerConstantsErrorMsg(
            "DEMON_FUSION_SKILLS must specify all 21 inheritance type skill "
            "mappings\n");
        return false;
      } else {
        size_t idx = 0;
        for (auto elem : strList) {
          auto vals = ToIntegerRange<uint32_t>(elem.C(), success);
          if (vals.size() != 3) {
            LogServerConstantsErrorMsg(
                "DEMON_FUSION_SKILLS element encountered with level count "
                "other than 3\n");
            return false;
          }

          size_t subIdx = 0;
          for (uint32_t val : vals) {
            sConstants.DEMON_FUSION_SKILLS[idx][subIdx++] = val;
          }

          idx++;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("DEMON_FUSION_SKILLS not found\n");
    return false;
  }

  complexIter = complexConstants.find("DEMON_QUEST_XP");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load DEMON_QUEST_XP\n");
      return false;
    } else {
      for (auto elem : strList) {
        uint32_t xp = 0;
        if (LoadInteger(elem.C(), xp)) {
          sConstants.DEMON_QUEST_XP.push_back(xp);
        } else {
          LogServerConstantsErrorMsg(
              "Failed to load an entry in DEMON_QUEST_XP\n");
          return false;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("DEMON_QUEST_XP not found\n");
    return false;
  }

  complexIter = complexConstants.find("DEPO_MAP_DEMON");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.DEPO_MAP_DEMON)) {
      LogServerConstantsErrorMsg("Failed to load DEPO_MAP_DEMON\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("DEPO_MAP_DEMON not found\n");
    return false;
  }

  complexIter = complexConstants.find("DEPO_MAP_ITEM");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.DEPO_MAP_ITEM)) {
      LogServerConstantsErrorMsg("Failed to load DEPO_MAP_ITEM\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("DEPO_MAP_ITEM not found\n");
    return false;
  }

  complexIter = complexConstants.find("EQUIP_MOD_EDIT_ITEMS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load EQUIP_MOD_EDIT_ITEMS\n");
      return false;
    }

    for (auto pair : map) {
      uint32_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load EQUIP_MOD_EDIT_ITEMS key\n");
        return false;
      } else if (sConstants.EQUIP_MOD_EDIT_ITEMS.find(key) !=
                 sConstants.EQUIP_MOD_EDIT_ITEMS.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate EQUIP_MOD_EDIT_ITEMS key encountered\n");
        return false;
      } else if (!ToIntegerArray(sConstants.EQUIP_MOD_EDIT_ITEMS[key],
                                 libcomp::String(pair.second).Split(","))) {
        LogServerConstantsErrorMsg(
            "Failed to load an element in EQUIP_MOD_EDIT_ITEMS\n");
        return false;
      }
    }
  } else {
    LogServerConstantsErrorMsg("EQUIP_MOD_EDIT_ITEMS not found\n");
    return false;
  }

  complexIter = complexConstants.find("FUSION_BOOST_SKILLS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load FUSION_BOOST_SKILLS\n");
      return false;
    }

    for (auto pair : map) {
      uint32_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load FUSION_BOOST_SKILLS key\n");
        return false;
      } else if (sConstants.FUSION_BOOST_SKILLS.find(key) !=
                 sConstants.FUSION_BOOST_SKILLS.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate FUSION_BOOST_SKILLS key encountered\n");
        return false;
      } else if (!ToIntegerArray(sConstants.FUSION_BOOST_SKILLS[key],
                                 libcomp::String(pair.second).Split(","))) {
        LogServerConstantsErrorMsg(
            "Failed to load an element in FUSION_BOOST_SKILLS\n");
        return false;
      }
    }
  } else {
    LogServerConstantsErrorMsg("FUSION_BOOST_SKILLS not found\n");
    return false;
  }

  complexIter = complexConstants.find("FUSION_BOOST_STATUSES");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.FUSION_BOOST_STATUSES)) {
      LogServerConstantsErrorMsg("Failed to load FUSION_BOOST_STATUSES\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("FUSION_BOOST_STATUSES not found\n");
    return false;
  }

  complexIter = complexConstants.find("LEVELUP_STATUSES");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.LEVELUP_STATUSES)) {
      LogServerConstantsErrorMsg("Failed to load LEVELUP_STATUSES\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("LEVELUP_STATUSES not found\n");
    return false;
  }

  complexIter = complexConstants.find("QUEST_BONUS");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.QUEST_BONUS)) {
      LogServerConstantsErrorMsg("Failed to load QUEST_BONUS\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("QUEST_BONUS not found\n");
    return false;
  }

  complexIter = complexConstants.find("RATE_SCALING_ITEMS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load RATE_SCALING_ITEMS\n");
      return false;
    } else {
      if (strList.size() != 4) {
        LogServerConstantsErrorMsg(
            "RATE_SCALING_ITEMS must specify items for each of the 4 types\n");
        return false;
      } else {
        size_t idx = 0;
        for (auto elemStr : strList) {
          if (!elemStr.IsEmpty()) {
            for (uint32_t p : ToIntegerRange<uint32_t>(elemStr.C(), success)) {
              sConstants.RATE_SCALING_ITEMS[idx].push_back(p);
            }

            if (!success) {
              LogServerConstantsErrorMsg(
                  "Failed to load an element in RATE_SCALING_ITEMS\n");
              return false;
            }
          }

          idx++;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("RATE_SCALING_ITEMS not found\n");
    return false;
  }

  complexIter = complexConstants.find("REUNION_EXTRACT_ITEMS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load REUNION_EXTRACT_ITEMS\n");
      return false;
    } else {
      for (auto elem : strList) {
        uint32_t id = 0;
        if (LoadInteger(elem.C(), id)) {
          sConstants.REUNION_EXTRACT_ITEMS.push_back(id);
        } else {
          LogServerConstantsErrorMsg(
              "Failed to load an entry in REUNION_EXTRACT_ITEMS\n");
          return false;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("REUNION_EXTRACT_ITEMS not found\n");
    return false;
  }

  complexIter = complexConstants.find("ROLLBACK_PG_ITEMS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load ROLLBACK_PG_ITEMS\n");
      return false;
    } else {
      for (auto elem : strList) {
        uint32_t id = 0;
        if (LoadInteger(elem.C(), id)) {
          sConstants.ROLLBACK_PG_ITEMS.push_back(id);
        } else {
          LogServerConstantsErrorMsg(
              "Failed to load an entry in ROLLBACK_PG_ITEMS\n");
          return false;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("ROLLBACK_PG_ITEMS not found\n");
    return false;
  }

  complexIter = complexConstants.find("SPIRIT_FUSION_BOOST");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load SPIRIT_FUSION_BOOST\n");
      return false;
    }

    for (auto pair : map) {
      uint32_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load SPIRIT_FUSION_BOOST key\n");
        return false;
      } else if (sConstants.SPIRIT_FUSION_BOOST.find(key) !=
                 sConstants.SPIRIT_FUSION_BOOST.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate SPIRIT_FUSION_BOOST key encountered\n");
        return false;
      } else if (!ToIntegerArray(sConstants.SPIRIT_FUSION_BOOST[key],
                                 libcomp::String(pair.second).Split(","))) {
        LogServerConstantsErrorMsg(
            "Failed to load an element in SPIRIT_FUSION_BOOST\n");
        return false;
      }
    }
  } else {
    LogServerConstantsErrorMsg("SPIRIT_FUSION_BOOST not found\n");
    return false;
  }

  complexIter = complexConstants.find("SYNTH_SKILLS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load SYNTH_SKILLS\n");
      return false;
    } else {
      if (strList.size() != 5) {
        LogServerConstantsErrorMsg(
            "SYNTH_SKILLS must specify all five skill IDs\n");
        return false;
      } else {
        size_t idx = 0;
        for (auto elem : strList) {
          uint32_t skillID = 0;
          if (LoadInteger(elem.C(), skillID)) {
            sConstants.SYNTH_SKILLS[idx] = skillID;
          } else {
            LogServerConstantsErrorMsg(
                "Failed to load a skill ID in SLOT_MOD_ITEMS\n");
            return false;
          }

          idx++;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("SYNTH_SKILLS not found\n");
    return false;
  }

  complexIter = complexConstants.find("TEAM_STATUS_COOLDOWN");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map) ||
        !LoadIntegerMap(map, sConstants.TEAM_STATUS_COOLDOWN)) {
      LogServerConstantsErrorMsg("Failed to load TEAM_STATUS_COOLDOWN\n");
      return false;
    }
  } else {
    LogServerConstantsErrorMsg("TEAM_STATUS_COOLDOWN not found\n");
    return false;
  }

  complexIter = complexConstants.find("TEAM_VALUABLES");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load TEAM_VALUABLES\n");
      return false;
    }

    for (auto pair : map) {
      int8_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg("Failed to load TEAM_VALUABLES key\n");
        return false;
      } else if (sConstants.TEAM_VALUABLES.find(key) !=
                 sConstants.TEAM_VALUABLES.end()) {
        LogServerConstantsErrorMsg(
            "Duplicate TEAM_VALUABLES key encountered\n");
        return false;
      } else {
        if (!pair.second.empty()) {
          for (uint16_t p : ToIntegerRange<uint16_t>(pair.second, success)) {
            sConstants.TEAM_VALUABLES[key].push_back(p);
          }

          if (!success) {
            LogServerConstantsErrorMsg(
                "Failed to load an element in TEAM_VALUABLES\n");
            return false;
          }
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("TEAM_VALUABLES not found\n");
    return false;
  }

  complexIter = complexConstants.find("TRIFUSION_SPECIAL_DARK");
  if (complexIter != complexConstants.end()) {
    std::unordered_map<std::string, std::string> map;
    if (!LoadKeyValueStrings(complexIter->second, map)) {
      LogServerConstantsErrorMsg("Failed to load TRIFUSION_SPECIAL_DARK\n");
      return false;
    }

    for (auto pair : map) {
      uint8_t key;
      if (!LoadInteger(pair.first, key)) {
        LogServerConstantsErrorMsg(
            "Failed to load TRIFUSION_SPECIAL_DARK key\n");
        return false;
      } else {
        uint32_t val = 0;
        if (LoadInteger(pair.second, val)) {
          sConstants.TRIFUSION_SPECIAL_DARK.push_back(
              std::pair<uint8_t, uint32_t>(key, val));
        } else {
          LogServerConstantsErrorMsg(
              "Failed to load an element in TRIFUSION_SPECIAL_DARK\n");
          return false;
        }
      }
    }

    sConstants.TRIFUSION_SPECIAL_DARK.sort(
        [](const std::pair<uint8_t, uint32_t>& a,
           const std::pair<uint8_t, uint32_t>& b) {
          return a.first < b.first;
        });
  } else {
    LogServerConstantsErrorMsg("TRIFUSION_SPECIAL_DARK not found\n");
    return false;
  }

  complexIter = complexConstants.find("TRIFUSION_SPECIAL_ELEMENTAL");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg(
          "Failed to load TRIFUSION_SPECIAL_ELEMENTAL\n");
      return false;
    } else {
      if (strList.size() != 6) {
        LogServerConstantsErrorMsg(
            "TRIFUSION_SPECIAL_ELEMENTAL must specify all 6 two elemental "
            "combinations\n");
        return false;
      } else {
        size_t idx = 0;
        for (auto elem : strList) {
          if (!ToIntegerArray(sConstants.TRIFUSION_SPECIAL_ELEMENTAL[idx++],
                              elem.Split(","))) {
            LogServerConstantsErrorMsg(
                "Failed to load an element in TRIFUSION_SPECIAL_ELEMENTAL\n");
            return false;
          }
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("TRIFUSION_SPECIAL_ELEMENTAL not found\n");
    return false;
  }

  complexIter = complexConstants.find("VA_ADD_ITEMS");
  if (complexIter != complexConstants.end()) {
    std::list<String> strList;
    if (!LoadStringList(complexIter->second, strList)) {
      LogServerConstantsErrorMsg("Failed to load VA_ADD_ITEMS\n");
      return false;
    } else {
      for (auto elemStr : strList) {
        uint32_t entry = 0;
        if (LoadInteger(elemStr.C(), entry)) {
          sConstants.VA_ADD_ITEMS.insert(entry);
        } else {
          LogServerConstantsErrorMsg(
              "Failed to load an element in VA_ADD_ITEMS\n");
          return false;
        }
      }
    }
  } else {
    LogServerConstantsErrorMsg("VA_ADD_ITEMS not found\n");
    return false;
  }

  //
  // API Admin Levels
  //
  success &= LoadInteger(constants["API_ADMIN_LVL_CREATE_PROMO"],
                         sConstants.API_ADMIN_LVL_CREATE_PROMO);
  success &= LoadInteger(constants["API_ADMIN_LVL_DELETE_ACCOUNT"],
                         sConstants.API_ADMIN_LVL_DELETE_ACCOUNT);
  success &= LoadInteger(constants["API_ADMIN_LVL_DELETE_PROMO"],
                         sConstants.API_ADMIN_LVL_DELETE_PROMO);
  success &= LoadInteger(constants["API_ADMIN_LVL_GET_ACCOUNT"],
                         sConstants.API_ADMIN_LVL_GET_ACCOUNT);
  success &= LoadInteger(constants["API_ADMIN_LVL_GET_ACCOUNTS"],
                         sConstants.API_ADMIN_LVL_GET_ACCOUNTS);
  success &= LoadInteger(constants["API_ADMIN_LVL_GET_PROMOS"],
                         sConstants.API_ADMIN_LVL_GET_PROMOS);
  success &= LoadInteger(constants["API_ADMIN_LVL_KICK_PLAYER"],
                         sConstants.API_ADMIN_LVL_KICK_PLAYER);
  success &= LoadInteger(constants["API_ADMIN_LVL_MESSAGE_WORLD"],
                         sConstants.API_ADMIN_LVL_MESSAGE_WORLD);
  success &= LoadInteger(constants["API_ADMIN_LVL_ONLINE"],
                         sConstants.API_ADMIN_LVL_ONLINE);
  success &= LoadInteger(constants["API_ADMIN_LVL_POST_ITEMS"],
                         sConstants.API_ADMIN_LVL_POST_ITEMS);
  success &= LoadInteger(constants["API_ADMIN_LVL_UPDATE_ACCOUNT"],
                         sConstants.API_ADMIN_LVL_UPDATE_ACCOUNT);

  //
  // GM Command Levels
  //
  success &=
      LoadInteger(constants["GM_CMD_LVL_ADD_CP"], sConstants.GM_CMD_LVL_ADD_CP);
  success &= LoadInteger(constants["GM_CMD_LVL_ANNOUNCE"],
                         sConstants.GM_CMD_LVL_ANNOUNCE);
  success &=
      LoadInteger(constants["GM_CMD_LVL_BAN"], sConstants.GM_CMD_LVL_BAN);
  success &= LoadInteger(constants["GM_CMD_LVL_BATTLE_POINTS"],
                         sConstants.GM_CMD_LVL_BATTLE_POINTS);
  success &=
      LoadInteger(constants["GM_CMD_LVL_BETHEL"], sConstants.GM_CMD_LVL_BETHEL);
  success &=
      LoadInteger(constants["GM_CMD_LVL_COIN"], sConstants.GM_CMD_LVL_COIN);
  success &= LoadInteger(constants["GM_CMD_LVL_CONTRACT"],
                         sConstants.GM_CMD_LVL_CONTRACT);
  success &= LoadInteger(constants["GM_CMD_LVL_COUNTER"],
                         sConstants.GM_CMD_LVL_COUNTER);
  success &=
      LoadInteger(constants["GM_CMD_LVL_COWRIE"], sConstants.GM_CMD_LVL_COWRIE);
  success &=
      LoadInteger(constants["GM_CMD_LVL_CRASH"], sConstants.GM_CMD_LVL_CRASH);
  success &= LoadInteger(constants["GM_CMD_LVL_DIGITALIZE_POINTS"],
                         sConstants.GM_CMD_LVL_DIGITALIZE_POINTS);
  success &=
      LoadInteger(constants["GM_CMD_LVL_EFFECT"], sConstants.GM_CMD_LVL_EFFECT);
  success &= LoadInteger(constants["GM_CMD_LVL_ENCHANT"],
                         sConstants.GM_CMD_LVL_ENCHANT);
  success &=
      LoadInteger(constants["GM_CMD_LVL_ENEMY"], sConstants.GM_CMD_LVL_ENEMY);
  success &=
      LoadInteger(constants["GM_CMD_LVL_EVENT"], sConstants.GM_CMD_LVL_EVENT);
  success &= LoadInteger(constants["GM_CMD_LVL_EXPERTISE_EXTEND"],
                         sConstants.GM_CMD_LVL_EXPERTISE_EXTEND);
  success &= LoadInteger(constants["GM_CMD_LVL_EXPERTISE_SET"],
                         sConstants.GM_CMD_LVL_EXPERTISE_SET);
  success &= LoadInteger(constants["GM_CMD_LVL_FAMILIARITY"],
                         sConstants.GM_CMD_LVL_FAMILIARITY);
  success &=
      LoadInteger(constants["GM_CMD_LVL_FLAG"], sConstants.GM_CMD_LVL_FLAG);
  success &= LoadInteger(constants["GM_CMD_LVL_FUSION_GAUGE"],
                         sConstants.GM_CMD_LVL_FUSION_GAUGE);
  success &=
      LoadInteger(constants["GM_CMD_LVL_GOTO"], sConstants.GM_CMD_LVL_GOTO);
  success &= LoadInteger(constants["GM_CMD_LVL_GRADE_POINTS"],
                         sConstants.GM_CMD_LVL_GRADE_POINTS);
  success &=
      LoadInteger(constants["GM_CMD_LVL_HELP"], sConstants.GM_CMD_LVL_HELP);
  success &= LoadInteger(constants["GM_CMD_LVL_HOMEPOINT"],
                         sConstants.GM_CMD_LVL_HOMEPOINT);
  success &= LoadInteger(constants["GM_CMD_LVL_INSTANCE"],
                         sConstants.GM_CMD_LVL_INSTANCE);
  success &=
      LoadInteger(constants["GM_CMD_LVL_ITEM"], sConstants.GM_CMD_LVL_ITEM);
  success &=
      LoadInteger(constants["GM_CMD_LVL_KICK"], sConstants.GM_CMD_LVL_KICK);
  success &= LoadInteger(constants["GM_CMD_LVL_LEVEL_UP"],
                         sConstants.GM_CMD_LVL_LEVEL_UP);
  success &=
      LoadInteger(constants["GM_CMD_LVL_LNC"], sConstants.GM_CMD_LVL_LNC);
  success &=
      LoadInteger(constants["GM_CMD_LVL_MAP"], sConstants.GM_CMD_LVL_MAP);
  success &=
      LoadInteger(constants["GM_CMD_LVL_ONLINE"], sConstants.GM_CMD_LVL_ONLINE);
  success &= LoadInteger(constants["GM_CMD_LVL_PENALTY_RESET"],
                         sConstants.GM_CMD_LVL_PENALTY_RESET);
  success &=
      LoadInteger(constants["GM_CMD_LVL_PLUGIN"], sConstants.GM_CMD_LVL_PLUGIN);
  success &= LoadInteger(constants["GM_CMD_LVL_POSITION"],
                         sConstants.GM_CMD_LVL_POSITION);
  success &=
      LoadInteger(constants["GM_CMD_LVL_POST"], sConstants.GM_CMD_LVL_POST);
  success &= LoadInteger(constants["GM_CMD_LVL_REPORTED"],
                         sConstants.GM_CMD_LVL_REPORTED);
  success &= LoadInteger(constants["GM_CMD_LVL_RESOLVE"],
                         sConstants.GM_CMD_LVL_RESOLVE);
  success &= LoadInteger(constants["GM_CMD_LVL_REUNION"],
                         sConstants.GM_CMD_LVL_REUNION);
  success &=
      LoadInteger(constants["GM_CMD_LVL_QUEST"], sConstants.GM_CMD_LVL_QUEST);
  success &=
      LoadInteger(constants["GM_CMD_LVL_SCRAP"], sConstants.GM_CMD_LVL_SCRAP);
  success &=
      LoadInteger(constants["GM_CMD_LVL_SKILL"], sConstants.GM_CMD_LVL_SKILL);
  success &= LoadInteger(constants["GM_CMD_LVL_SKILL_POINT"],
                         sConstants.GM_CMD_LVL_SKILL_POINT);
  success &= LoadInteger(constants["GM_CMD_LVL_SLOT_ADD"],
                         sConstants.GM_CMD_LVL_SLOT_ADD);
  success &= LoadInteger(constants["GM_CMD_LVL_SOUL_POINTS"],
                         sConstants.GM_CMD_LVL_SOUL_POINTS);
  success &=
      LoadInteger(constants["GM_CMD_LVL_SPAWN"], sConstants.GM_CMD_LVL_SPAWN);
  success &=
      LoadInteger(constants["GM_CMD_LVL_SPEED"], sConstants.GM_CMD_LVL_SPEED);
  success &=
      LoadInteger(constants["GM_CMD_LVL_SPIRIT"], sConstants.GM_CMD_LVL_SPIRIT);
  success &= LoadInteger(constants["GM_CMD_LVL_SUPPORT"],
                         sConstants.GM_CMD_LVL_SUPPORT);
  success &= LoadInteger(constants["GM_CMD_LVL_TICKER_MESSAGE"],
                         sConstants.GM_CMD_LVL_TICKER_MESSAGE);
  success &=
      LoadInteger(constants["GM_CMD_LVL_TITLE"], sConstants.GM_CMD_LVL_TITLE);
  success &= LoadInteger(constants["GM_CMD_LVL_TOKUSEI"],
                         sConstants.GM_CMD_LVL_TOKUSEI);
  success &= LoadInteger(constants["GM_CMD_LVL_VALUABLE"],
                         sConstants.GM_CMD_LVL_VALUABLE);
  success &= LoadInteger(constants["GM_CMD_LVL_WORLD_TIME"],
                         sConstants.GM_CMD_LVL_WORLD_TIME);
  success &= LoadInteger(constants["GM_CMD_LVL_ZIOTITE"],
                         sConstants.GM_CMD_LVL_ZIOTITE);
  success &=
      LoadInteger(constants["GM_CMD_LVL_ZONE"], sConstants.GM_CMD_LVL_ZONE);
  success &= LoadInteger(constants["GM_CMD_LVL_XP"], sConstants.GM_CMD_LVL_XP);

  return success;
}

const ServerConstants::Data& ServerConstants::GetConstants() {
  return sConstants;
}

bool ServerConstants::LoadString(const std::string& value, String& prop) {
  prop = value;
  return !value.empty();
}

bool ServerConstants::LoadStringList(const tinyxml2::XMLElement* elem,
                                     std::list<String>& prop) {
  if (elem) {
    const tinyxml2::XMLElement* pElement = elem;
    if (std::string(pElement->Value()) != "element") {
      return false;
    } else {
      while (pElement) {
        libcomp::String elemStr(pElement && pElement->FirstChild()->ToText()
                                    ? pElement->FirstChild()->ToText()->Value()
                                    : "");
        if (!elemStr.IsEmpty()) {
          prop.push_back(elemStr);
        }

        pElement = pElement->NextSiblingElement("element");
      }
    }
  }

  return true;
}

bool ServerConstants::LoadKeyValueStrings(
    const tinyxml2::XMLElement* elem,
    std::unordered_map<std::string, std::string>& map) {
  if (elem) {
    const tinyxml2::XMLElement* pPair = elem;
    if (std::string(pPair->Value()) != "pair") {
      return false;
    } else {
      while (pPair) {
        const tinyxml2::XMLElement* pKey = pPair->FirstChildElement("key");
        const tinyxml2::XMLElement* pVal = pPair->FirstChildElement("value");

        std::string keyStr(pKey && pKey->FirstChild()->ToText()
                               ? pKey->FirstChild()->ToText()->Value()
                               : "");
        std::string valStr(pVal && pVal->FirstChild()->ToText()
                               ? pVal->FirstChild()->ToText()->Value()
                               : "");
        if (!keyStr.empty() || !valStr.empty()) {
          if (map.find(keyStr) != map.end()) {
            return false;
          }

          map[keyStr] = valStr;
        } else {
          return false;
        }

        pPair = pPair->NextSiblingElement("pair");
      }
    }
  } else {
    return false;
  }

  return true;
}
