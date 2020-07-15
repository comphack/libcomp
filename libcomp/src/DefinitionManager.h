/**
 * @file libcomp/src/DefinitionManager.h
 * @ingroup libcomp
 *
 * @author HACKfrost
 *
 * @brief Manages parsing and storing binary game data definitions.
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

#ifndef LIBCOMP_SRC_DEFINITIONMANAGER_H
#define LIBCOMP_SRC_DEFINITIONMANAGER_H

// libcomp Includes
#include "CString.h"
#include "Crypto.h"
#include "DataStore.h"
#include "MiCorrectTbl.h"
#include "Object.h"

// Standard C++11 Includes
#include <set>
#include <unordered_map>

namespace objects {
class EnchantSetData;
class EnchantSpecialData;
class MiAIData;
class MiBlendData;
class MiBlendExtData;
class MiCHouraiData;
class MiCItemData;
class MiCorrectTbl;
class MiCultureItemData;
class MiDevilBookData;
class MiDevilBoostData;
class MiDevilBoostExtraData;
class MiDevilBoostItemData;
class MiDevilData;
class MiDevilEquipmentData;
class MiDevilEquipmentItemData;
class MiDevilFusionData;
class MiDevilLVUpRateData;
class MiDisassemblyData;
class MiDisassemblyTriggerData;
class MiDynamicMapData;
class MiEnchantData;
class MiEquipmentSetData;
class MiExchangeData;
class MiExpertData;
class MiGuardianAssistData;
class MiGuardianLevelData;
class MiGuardianSpecialData;
class MiGuardianUnlockData;
class MiHNPCData;
class MiItemData;
class MiMissionData;
class MiMitamaReunionBonusData;
class MiMitamaReunionSetBonusData;
class MiMitamaUnionBonusData;
class MiModificationData;
class MiModificationExtEffectData;
class MiModificationExtRecipeData;
class MiModificationTriggerData;
class MiModifiedEffectData;
class MiNPCBarterData;
class MiNPCBarterConditionData;
class MiNPCBarterGroupData;
class MiONPCData;
class MiQuestBonusCodeData;
class MiQuestData;
class MiShopProductData;
class MiSItemData;
class MiSkillData;
class MiSpotData;
class MiSStatusData;
class MiStatusData;
class MiSynthesisData;
class MiTankData;
class MiTimeLimitData;
class MiTitleData;
class MiTriUnionSpecialData;
class MiUraFieldTowerData;
class MiWarpPointData;
class MiZoneData;
class QmpFile;
class Tokusei;
}  // namespace objects

namespace libcomp {

/**
 * Manager class responsible for loading binary files that are accessible
 * client side to use as server definitions.
 */
class DefinitionManager {
 public:
  /**
   * Create a new DefinitionManager.
   */
  DefinitionManager();

  /**
   * Clean up the DefinitionManager.
   */
  ~DefinitionManager();

  /**
   * Get the client-side AI definition corresponding to an ID
   * @param id Client-side AI to retrieve
   * @return Pointer to the matching client-side AI definition, null if
   *  it does not exist
   */
  const std::shared_ptr<objects::MiAIData> GetAIData(uint32_t id);

  /**
   * Get the item mixing definition corresponding to an ID
   * @param id Item mixing ID to retrieve
   * @return Pointer to the matching item mixing definition, null if
   *  it does not exist
   */
  const std::shared_ptr<objects::MiBlendData> GetBlendData(uint32_t id);

  /**
   * Get the item mixing extension definition corresponding to an ID
   * @param id Item mixing extension ID to retrieve
   * @return Pointer to the matching item mixing extension definition,
   *  null if it does not exist
   */
  const std::shared_ptr<objects::MiBlendExtData> GetBlendExtData(uint32_t id);

  /**
   * Get the client-side hourai NPC definitions by ID
   * @return Map of client-side hourai NPC definitions by ID
   */
  std::unordered_map<int8_t, std::shared_ptr<objects::MiCHouraiData>>
  GetCHouraiData();

  /**
   * Get the culture item definition corresponding to an ID
   * @param id Culture item ID to retrieve
   * @return Pointer to the matching culture item definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiCultureItemData> GetCultureItemData(
      uint32_t id);

  /**
   * Get the devil book definition corresponding to an ID
   * @param id Devil book ID to retrieve
   * @return Pointer to the matching devil book definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilBookData> GetDevilBookData(uint32_t id);

  /**
   * Get all devil book definitions by definition ID
   * @return Map of devil book definitions by ID
   */
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilBookData>>
  GetDevilBookData();

  /**
   * Get the devil boost definition corresponding to an ID
   * @param id Devil boost ID to retrieve
   * @return Pointer to the matching devil boost definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilBoostData> GetDevilBoostData(
      uint32_t id);

  /**
   * Get the devil boost extra definition corresponding to a stack ID
   * @param id Devil boost extra stack ID to retrieve
   * @return Pointer to the matching devil boost extra definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilBoostExtraData> GetDevilBoostExtraData(
      uint16_t id);

  /**
   * Get the devil boost item definition corresponding to an ID
   * @param id Devil boost item ID to retrieve
   * @return Pointer to the matching devil boost item definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilBoostItemData> GetDevilBoostItemData(
      uint32_t id);

  /**
   * Get the list of all stack IDs in a devil boost lot or any lower lot
   * that multiplies into supplied value (ex: 400 returns 100 and 200 lots)
   * @param count Boost count from a demon's benefit gauge
   * @return List of stack IDs
   */
  std::list<uint16_t> GetDevilBoostLotIDs(int32_t count);

  /**
   * Get the devil definition corresponding to an ID
   * @param id Devil ID to retrieve
   * @return Pointer to the matching devil definition, null if it does
   *  not exist
   */
  std::shared_ptr<objects::MiDevilData> GetDevilData(uint32_t id);

  /**
   * Get a devil definition corresponding to a name
   * @param name Devil name to retrieve
   * @return Pointer to the matching devil definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilData> GetDevilData(
      const libcomp::String& name);

  /**
   * Get the devil equipment definition corresponding to a skill ID
   * @param id Devil equipment skill ID to retrieve
   * @return Pointer to the matching devil equipment definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilEquipmentData> GetDevilEquipmentData(
      uint32_t skillID);

  /**
   * Get the devil equipment item definition corresponding to an ID
   * @param id Devil equipment item ID to retrieve
   * @return Pointer to the matching devil equipment item definition, null if
   *  it does not exist
   */
  const std::shared_ptr<objects::MiDevilEquipmentItemData>
  GetDevilEquipmentItemData(uint32_t itemID);

  /**
   * Get the fusion skill definition corresponding to an ID
   * @param id Fusion skill ID to retrieve
   * @return Pointer to the matching fusion skill definition, null if
   *  it does not exist
   */
  const std::shared_ptr<objects::MiDevilFusionData> GetDevilFusionData(
      uint32_t id);

  /**
   * Get set of devil fusion definition IDs that have an explicit demon
   * ID listed as a requirement.
   * @param demonID Definition ID of a demon
   * @return Set of devil fusion definition IDs
   */
  std::set<uint32_t> GetDevilFusionIDsByDemonID(uint32_t demonID);

  /**
   * Get the devil level up information corresponding to an ID
   * @param id Devil level up information ID to retrieve
   * @return Pointer to the matching devil level up information, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDevilLVUpRateData> GetDevilLVUpRateData(
      uint32_t id);

  /**
   * Get all devil level up information records
   * @return Map of devil level up information by growth type ID
   */
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilLVUpRateData>>
  GetAllDevilLVUpRateData();

  /**
   * Get the item disassembly definition corresponding to an ID
   * @param id Item disassembly ID to retrieve
   * @return Pointer to the matching item disassembly definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDisassemblyData> GetDisassemblyData(
      uint32_t id);

  /**
   * Get the item disassembly definition corresponding to an item ID
   * @param itemID Item ID to retrieve the disassembly from
   * @return Pointer to the matching item disassembly definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDisassemblyData> GetDisassemblyDataByItemID(
      uint32_t itemID);

  /**
   * Get the item disassembly trigger definition corresponding to an ID
   * @param id Item disassembly trigger ID to retrieve
   * @return Pointer to the matching item disassembly trigger definition,
   *  null if it does not exist
   */
  const std::shared_ptr<objects::MiDisassemblyTriggerData>
  GetDisassemblyTriggerData(uint32_t id);

  /**
   * Get all item IDs associated to disassembled items
   * @return List of all disassembled item IDs
   */
  const std::list<uint32_t> GetDisassembledItemIDs();

  /**
   * Get the dynamic map information corresponding to an ID
   * @param id Dynamic map information ID to retrieve
   * @return Pointer to the matching dynamic map information, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiDynamicMapData> GetDynamicMapData(
      uint32_t id);

  /**
   * Get the enchantment definition corresponding to an ID
   * @param id Enchantment ID to retrieve
   * @return Pointer to the matching enchantment definition, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiEnchantData> GetEnchantData(int16_t id);

  /**
   * Get a map of all enchantment definitions by ID
   * @return Map of all enchantment definitions by ID
   */
  std::unordered_map<int16_t, std::shared_ptr<objects::MiEnchantData>>
  GetAllEnchantData();

  /**
   * Get the enchantment definition corresponding to a demon ID
   * @param demonID Demon ID to retrieve a definition from
   * @return Pointer to the matching enchantment definition, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiEnchantData> GetEnchantDataByDemonID(
      uint32_t demonID);

  /**
   * Get the enchantment definition corresponding to an item ID
   * @param itemID Item ID to retrieve a definition from
   * @return Pointer to the matching enchantment definition, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiEnchantData> GetEnchantDataByItemID(
      uint32_t itemID);

  /**
   * Get the equipment set information corresponding to an ID
   * @param id Equipment set ID to retrieve
   * @return Pointer to the matching equipment set information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiEquipmentSetData> GetEquipmentSetData(
      uint32_t id);

  /**
   * Get the equipment set information corresponding to a piece of equipment
   * in the set
   * @param equipmentID Equipment ID to retrieve sets for
   * @return List of pointers to the matching equipment set information
   */
  std::list<std::shared_ptr<objects::MiEquipmentSetData>>
  GetEquipmentSetDataByItem(uint32_t equipmentID);

  /**
   * Get the item exchange data corresponding to an ID
   * @param id Exchange ID to retrieve
   * @return Pointer to the matching item exchange information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiExchangeData> GetExchangeData(uint32_t id);

  /**
   * Get the character expertise information corresponding to an ID
   * @param id Expertise ID to retrieve
   * @return Pointer to the matching character expertise information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiExpertData> GetExpertClassData(uint32_t id);

  /**
   * Get the list of fusion range level and demon ID pairs associated to
   * the supplied demon race
   * @param raceID Demon race ID
   * @return List of maximum fusion range level (key) and demon ID
   *  (value) pairs
   */
  const std::list<std::pair<uint8_t, uint32_t>> GetFusionRanges(uint8_t raceID);

  /**
   * Get the guardian assist information corresponding to an ID
   * @param id Guardian assist ID to retrieve
   * @return Pointer to the matching guardian assist information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiGuardianAssistData> GetGuardianAssistData(
      uint32_t id);

  /**
   * Get all guardian race IDs with defined level information
   * @return Set of all guardian race IDs
   */
  const std::set<uint8_t> GetGuardianRaceIDs();

  /**
   * Get the guardian level information corresponding to an ID
   * @param id Guardian level ID to retrieve
   * @return Pointer to the matching guardian level information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiGuardianLevelData> GetGuardianLevelData(
      uint32_t id);

  /**
   * Get the guardian special information corresponding to an ID
   * @param id Guardian special ID to retrieve
   * @return Pointer to the matching guardian special information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiGuardianSpecialData> GetGuardianSpecialData(
      uint32_t id);

  /**
   * Get the guardian unlock information corresponding to an ID
   * @param id Guardian unlock ID to retrieve
   * @return Pointer to the matching guardian unlock information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiGuardianUnlockData> GetGuardianUnlockData(
      uint32_t id);

  /**
   * Get the human NPC definition corresponding to an ID
   * @param id Human NPC ID to retrieve
   * @return Pointer to the matching human NPC definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiHNPCData> GetHNPCData(uint32_t id);

  /**
   * Get the item definition corresponding to an ID
   * @param id Item ID to retrieve
   * @return Pointer to the matching item definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiItemData> GetItemData(uint32_t id);

  /**
   * Get the item definition corresponding to a name
   * @param name Item name to retrieve
   * @return Pointer to the matching item definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiItemData> GetItemData(
      const libcomp::String& name);

  /**
   * Get the mission definition corresponding to an ID
   * @param id Mission ID to retrieve
   * @return Pointer to the matching mission definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiMissionData> GetMissionData(uint32_t id);

  /**
   * Get the mitama reunion bonus definition corresponding to an ID
   * @param id Mitama reunion bonus ID to retrieve
   * @return Pointer to the matching mitama reunion bonus definition,
   *  null if it does not exist
   */
  const std::shared_ptr<objects::MiMitamaReunionBonusData>
  GetMitamaReunionBonusData(uint32_t id);

  /**
   * Get all mitama reunion set bonus definitions by ID
   * @return Mitama reunion set bonus definitions by ID
   */
  std::unordered_map<uint32_t,
                     std::shared_ptr<objects::MiMitamaReunionSetBonusData>>
  GetMitamaReunionSetBonusData();

  /**
   * Get the mitama union bonus definition corresponding to an ID
   * @param id Mitama union bonus ID to retrieve
   * @return Pointer to the matching mitama union bonus definition,
   *  null if it does not exist
   */
  const std::shared_ptr<objects::MiMitamaUnionBonusData>
  GetMitamaUnionBonusData(uint32_t id);

  /**
   * Get the item modification definition corresponding to an ID
   * @param id Item modification ID to retrieve
   * @return Pointer to the matching item modification definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiModificationData> GetModificationData(
      uint32_t id);

  /**
   * Get the item modification definition corresponding to an item ID
   * @param itemID Item ID to retrieve the modification from
   * @return Pointer to the matching item modification definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiModificationData>
  GetModificationDataByItemID(uint32_t itemID);

  /**
   * Get the item modification extra effect definition corresponding to a
   * group ID, slot and subID
   * @param groupID Item modification extra effect groupID
   * @param groupID Item modification extra effect slot
   * @param groupID Item modification extra effect subID
   * @return Pointer to the matching item modification extra effect
   *  definition, null if it does not exist
   */
  const std::shared_ptr<objects::MiModificationExtEffectData>
  GetModificationExtEffectData(uint8_t groupID, uint8_t slot, uint16_t subID);

  /**
   * Get the item modification extra recipe definition corresponding to an ID
   * @param id Item modification extra recipe ID to retrieve
   * @return Pointer to the matching item modification extra recipe
   *  definition, null if it does not exist
   */
  const std::shared_ptr<objects::MiModificationExtRecipeData>
  GetModificationExtRecipeData(uint32_t id);

  /**
   * Get the item modification extra recipe definition corresponding to
   * an item ID
   * @param itemID Item ID to retrieve the modification from
   * @return Pointer to the matching item modification extra recipe
   *  definition, null if it does not exist
   */
  const std::shared_ptr<objects::MiModificationExtRecipeData>
  GetModificationExtRecipeDataByItemID(uint32_t itemID);

  /**
   * Get the item modification trigger definition corresponding to an ID
   * @param id Item modification trigger ID to retrieve
   * @return Pointer to the matching item modification trigger definition,
   *  null if it does not exist
   */
  const std::shared_ptr<objects::MiModificationTriggerData>
  GetModificationTriggerData(uint16_t id);

  /**
   * Get the item modification effect definition corresponding to an ID
   * @param id Item modification effect ID to retrieve
   * @return Pointer to the matching item modification effect definition,
   *  null if it does not exist
   */
  const std::shared_ptr<objects::MiModifiedEffectData> GetModifiedEffectData(
      uint16_t id);

  /**
   * Get the NPC barter definition corresponding to an ID
   * @param id NPC barter ID to retrieve
   * @return Pointer to the matching NPC barer definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiNPCBarterData> GetNPCBarterData(uint16_t id);

  /**
   * Get the NPC barter group definition corresponding to an ID
   * @param id NPC barter group ID to retrieve
   * @return Pointer to the matching NPC barer group definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiNPCBarterGroupData> GetNPCBarterGroupData(
      uint16_t id);

  /**
   * Get the NPC barter condition definition corresponding to an ID
   * @param id NPC barter condition ID to retrieve
   * @return Pointer to the matching NPC barter condition definition, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiNPCBarterConditionData>
  GetNPCBarterConditionData(uint16_t id);

  /**
   * Get the server object NPC definition corresponding to an ID
   * @param id Server object NPC ID to retrieve
   * @return Pointer to the matching server object NPC definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiONPCData> GetONPCData(uint32_t id);

  /**
   * Get the quest bonus code definition corresponding to an ID
   * @param id Quest bonus codeID to retrieve
   * @return Pointer to the matching quest bonus code, null if it does not
   *  exist
   */
  const std::shared_ptr<objects::MiQuestBonusCodeData> GetQuestBonusCodeData(
      uint32_t id);

  /**
   * Get the quest definition corresponding to an ID
   * @param id Quest ID to retrieve
   * @return Pointer to the matching quest, null if it does not exist
   */
  const std::shared_ptr<objects::MiQuestData> GetQuestData(uint32_t id);

  /**
   * Get the shop product definition corresponding to an ID
   * @param id Shop product ID to retrieve
   * @return Pointer to the matching shop product definition, null if it
   *  does not exist
   */
  const std::shared_ptr<objects::MiShopProductData> GetShopProductData(
      uint32_t id);

  /**
   * Get the s-item tokusei IDs corresponding to an item ID
   * @param id Item ID to retrieve
   * @return Set of tokusei mapped to the supplied item ID
   */
  std::set<int32_t> GetSItemTokusei(uint32_t id);

  /**
   * Get the skill definition corresponding to an ID
   * @param id Skill ID to retrieve
   * @return Pointer to the matching skill definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiSkillData> GetSkillData(uint32_t id);

  /**
   * Get all skill definition IDs that are mapped to the supplied function ID
   * @param fid Skill function ID
   * @return Set of skill definition IDs
   */
  std::set<uint32_t> GetFunctionIDSkills(uint16_t fid) const;

  /**
   * Get the spot data corresponding to a dynamic map ID
   * @param dynamicMapID ID of the spot data to retrieve
   * @return Map of spot definitions by spot ID for the specified dynamic map
   */
  const std::unordered_map<uint32_t, std::shared_ptr<objects::MiSpotData>>
  GetSpotData(uint32_t dynamicMapID);

  /**
   * Get the s-status definition corresponding to an ID
   * @param id S-status ID to retrieve
   * @return Pointer to the matching s-status definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiSStatusData> GetSStatusData(uint32_t id);

  /**
   * Get the status definition corresponding to an ID
   * @param id Status ID to retrieve
   * @return Pointer to the matching status definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiStatusData> GetStatusData(uint32_t id);

  /**
   * Get the synthesis definition corresponding to an ID
   * @param id Synthesis ID to retrieve
   * @return Pointer to the matching synthesis definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiSynthesisData> GetSynthesisData(uint32_t id);

  /**
   * Get all synthesis definitions by ID
   * @return All synthesis definitions by ID
   */
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiSynthesisData>>
  GetAllSynthesisData();

  /**
   * Get the map of all material tank item definitions by ID
   * @return Map of pointers to material tank item definitions by ID
   */
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiTankData>>
  GetTankData();

  /**
   * Get the time limit definition corresponding to an ID
   * @param id Time limit ID to retrieve
   * @return Pointer to the matching time limit definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiTimeLimitData> GetTimeLimitData(uint32_t id);

  /**
   * Get the title definition corresponding to an ID
   * @param id Title ID to retrieve
   * @return Pointer to the matching title definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiTitleData> GetTitleData(int16_t id);

  /**
   * Get the set of all (non-special) title IDs
   * @return Set of title IDs
   */
  std::set<int16_t> GetTitleIDs();

  /**
   * Get the list of pointers to special fusion definitions by the ID of
   * a source demon involved
   * @param sourceDemonTypeID ID of a source demon for the special fusion
   * @return List of pointers to special fusion definitions
   */
  const std::list<std::shared_ptr<objects::MiTriUnionSpecialData>>
  GetTriUnionSpecialData(uint32_t sourceDemonTypeID);

  /**
   * Get the (Diaspora) ura field tower corresponding to an ID
   * @param id Ura field tower ID to retrieve
   * @return Pointer to the matching ura field tower information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiUraFieldTowerData> GetUraFieldTowerData(
      uint32_t dungeonID, uint32_t id);

  /**
   * Get the warp point corresponding to an ID
   * @param id Warp point ID to retrieve
   * @return Pointer to the matching warp point information, null
   *  if it does not exist
   */
  const std::shared_ptr<objects::MiWarpPointData> GetWarpPointData(uint32_t id);

  /**
   * Get the zone definition corresponding to an ID
   * @param id Zone ID to retrieve
   * @return Pointer to the matching zone definition, null if it does
   *  not exist
   */
  const std::shared_ptr<objects::MiZoneData> GetZoneData(uint32_t id);

  /**
   * Get an enchant set by definition ID
   * @param id Definition ID of an enchant set to load
   * @return Pointer to the enchant set matching the specified id
   */
  const std::shared_ptr<objects::EnchantSetData> GetEnchantSetData(uint32_t id);

  /**
   * Get the enchant set information corresponding to an effect
   * @param effectID Effect ID to retrieve sets for
   * @return List of pointers to the matching enchant set information
   */
  std::list<std::shared_ptr<objects::EnchantSetData>> GetEnchantSetDataByEffect(
      int16_t effectID);

  /**
   * Get all enchant set definitions by ID
   * @return Map of all enchant set definitions by ID
   */
  const std::unordered_map<uint32_t, std::shared_ptr<objects::EnchantSetData>>
  GetAllEnchantSetData();

  /**
   * Get a special enchant by definition ID
   * @param id Definition ID of a special enchant to load
   * @return Pointer to the special enchant matching the specified id
   */
  const std::shared_ptr<objects::EnchantSpecialData> GetEnchantSpecialData(
      uint32_t id);

  /**
   * Get the special enchant information corresponding to an input item type
   * @param itemID Input item ID to retrieve special enchants for
   * @return List of pointers to the matching special enchant information
   */
  std::list<std::shared_ptr<objects::EnchantSpecialData>>
  GetEnchantSpecialDataByInputItem(uint32_t itemID);

  /**
   * Get a tokusei by definition ID
   * @param id Definition ID of a tokusei to load
   * @return Pointer to the tokusei matching the specified id
   */
  const std::shared_ptr<objects::Tokusei> GetTokuseiData(int32_t id);

  /**
   * Get all tokusei definitions by ID
   * @return Map of all tokusei definitions by ID
   */
  const std::unordered_map<int32_t, std::shared_ptr<objects::Tokusei>>
  GetAllTokuseiData();

  /**
   * Load all binary data definitions
   * @param pDataStore Pointer to the datastore to load binary files from
   * @return true on success, false on failure
   */
  bool LoadAllData(DataStore* pDataStore);

  /**
   * Load the binary data definitions of the specified type
   * @param pDataStore Pointer to the datastore to load binary file from
   * @return true on success, false on failure
   */
  template <class T>
  bool LoadData(DataStore* pDataStore);

  /**
   * Load the QMP file with the specified filename from the supplied datastore.
   * Unlike other "load" functions on the manager, this information is not
   * cached.
   * @param fileName Name of the QMP file to load, including the file extension
   * @param pDataStore Pointer to the datastore to load file from
   * @return Pointer to the structure holding the parsed file information or
   *  null on failure
   */
  std::shared_ptr<objects::QmpFile> LoadQmpFile(const libcomp::String& fileName,
                                                DataStore* pDataStore);

  /**
   * Register a server side definition into the manager from an external
   * source.
   * @param record Pointer to the record of the templated type
   * @return true if the record was registered successfully, false if it
   *  was not
   */
  template <class T>
  bool RegisterServerSideDefinition(const std::shared_ptr<T>& record);

 protected:
  /**
   * Load a binary file from the specified data store location
   * @param pDataStore Pointer to a data store location to check
   *  for the file
   * @param binaryFile Relative file path to a binary file
   * @param decrypt true if the file is encrypted and must be
   *  decrypted first, false if it can just be loaded
   * @param tablesExpected Number of tables expected in the file
   *  format
   * @param records Output list to load records into
   * @param printResults Optional parameter to disable success/fail
   *  debug messages
   * @return true if the file was loaded, false if it was not
   */
  template <class T>
  bool LoadBinaryData(DataStore* pDataStore, const libcomp::String& binaryFile,
                      bool decrypt, uint16_t tablesExpected,
                      std::list<std::shared_ptr<T>>& records,
                      bool printResults = true) {
    std::vector<char> data;

    auto path = libcomp::String("/BinaryData/") + binaryFile;

    if (decrypt) {
      data = pDataStore->DecryptFile(path);
    } else {
      data = pDataStore->ReadFile(path);
    }

    if (data.empty()) {
      if (printResults) {
        PrintLoadResult(binaryFile, false, 0, 0);
      }
      return false;
    }

    std::stringstream ss(std::string(data.begin(), data.end()));
    libcomp::ObjectInStream ois(ss);

    uint16_t entryCount, tableCount;
    if (!LoadBinaryDataHeader(ois, binaryFile, tablesExpected, entryCount,
                              tableCount)) {
      return false;
    }

    size_t dynamicCounts = (size_t)(entryCount * tableCount);
    for (uint16_t i = 0; i < dynamicCounts; i++) {
      uint16_t ds;
      ois.stream.read(reinterpret_cast<char*>(&ds), sizeof(ds));
      ois.dynamicSizes.push_back(ds);
    }

    for (uint16_t i = 0; i < entryCount; i++) {
      auto entry = std::shared_ptr<T>(new T);

      if (!entry->Load(ois)) {
        if (printResults) {
          PrintLoadResult(binaryFile, false, entryCount, records.size());
        }
        return false;
      }

      records.push_back(entry);
    }

    bool success = entryCount == records.size() && ois.stream.good();
    if (printResults) {
      PrintLoadResult(binaryFile, success, entryCount, records.size());
    }

    return success;
  }

  /**
   * Load the data header containing the number of entries and
   * tables that make up the format of the rest of the file
   * @param ois Binary stream of the file's contents
   * @param binaryFile Relative file path to a binary file
   * @param tablesExpected Number of tables expected in the file
   *  format.  If this number does not match the tableCount result
   *  an error will be printed and will result in failure.
   * @param entryCount Output param containing the number of
   *  individual records contained in the file
   * @param tableCount Output param containing the number of
   *  dynamically sized "table" data members for each entry
   * @return true if the data header was read, false if it failed
   */
  bool LoadBinaryDataHeader(libcomp::ObjectInStream& ois,
                            const libcomp::String& binaryFile,
                            uint16_t tablesExpected, uint16_t& entryCount,
                            uint16_t& tableCount);

  /**
   * Utility function to print the result of loading a binary file
   * @param binaryFile Relative file path to a binary file
   * @param success true if the load succeeded, false if it failed
   * @param entriesExpected Number of entries that were supposed to
   *  be in the file according to the data header
   * @param loadedEntries NUmber of entries successfully loaded
   */
  void PrintLoadResult(const libcomp::String& binaryFile, bool success,
                       uint16_t entriesExpected, size_t loadedEntries);

  /**
   * Utility function to pull the templated type from a standard
   * definition map of ID to a pointer of that type
   * @param id ID of the definition to retrieve from the map
   * @param data Map to retrieve the data from
   * @return Pointer to the record in the map matching the
   *  supplied ID, null if it does not exist
   */
  template <class X, class T>
  std::shared_ptr<T> GetRecordByID(
      X id, std::unordered_map<X, std::shared_ptr<T>>& data) {
    auto iter = data.find(id);
    if (iter != data.end()) {
      return iter->second;
    }

    return nullptr;
  }

 private:
  /// Map of client-side AI definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiAIData>> mAIData;

  /// Map of item mixing definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiBlendData>>
      mBlendData;

  /// Map of item mixing extension definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiBlendExtData>>
      mBlendExtData;

  /// Map of client-side hourai NPC definitions by ID
  std::unordered_map<int8_t, std::shared_ptr<objects::MiCHouraiData>>
      mCHouraiData;

  /// Map of culture item definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiCultureItemData>>
      mCultureItemData;

  /// Map of item names to IDs
  std::unordered_map<libcomp::String, uint32_t> mCItemNameLookup;

  /// Map of devil book definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilBookData>>
      mDevilBookData;

  /// Map of devil boost definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilBoostData>>
      mDevilBoostData;

  /// Map of devil boost extra definitions by ID
  std::unordered_map<uint16_t, std::shared_ptr<objects::MiDevilBoostExtraData>>
      mDevilBoostExtraData;

  /// Map of devil boost item definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilBoostItemData>>
      mDevilBoostItemData;

  /// Map of lot IDs to devil boost extra stack IDs in that lot
  std::unordered_map<int32_t, std::list<uint16_t>> mDevilBoostLots;

  /// Map of devil definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilData>>
      mDevilData;

  /// Map of devil equipment definitions by skill ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilEquipmentData>>
      mDevilEquipmentData;

  /// Map of devil equipment definitions by item ID
  std::unordered_map<uint32_t,
                     std::shared_ptr<objects::MiDevilEquipmentItemData>>
      mDevilEquipmentItemData;

  /// Map of devil fusion definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilFusionData>>
      mDevilFusionData;

  /// Map of devil fusion definition IDs mapped by demon IDs listed as
  /// explicit requirements for the skill
  std::unordered_map<uint32_t, std::set<uint32_t>> mDevilFusionLookup;

  /// Map of devil names to IDs which are NOT unique across entries
  std::unordered_map<libcomp::String, uint32_t> mDevilNameLookup;

  /// Map of devil level up information by growth type ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilLVUpRateData>>
      mDevilLVUpRateData;

  /// Map of item disassembly definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDisassemblyData>>
      mDisassemblyData;

  /// Map of item disassembly definition IDs by item ID
  std::unordered_map<uint32_t, uint32_t> mDisassemblyLookup;

  /// Map of item disassembly trigger definitions by ID
  std::unordered_map<uint32_t,
                     std::shared_ptr<objects::MiDisassemblyTriggerData>>
      mDisassemblyTriggerData;

  /// List of all disassembled item IDs (or "elements")
  std::list<uint32_t> mDisassembledItemIDs;

  /// Map of dynamic map information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiDynamicMapData>>
      mDynamicMapData;

  /// Map of enchantment definitions by ID
  std::unordered_map<int16_t, std::shared_ptr<objects::MiEnchantData>>
      mEnchantData;

  /// Map of enchantment IDs by demon ID
  std::unordered_map<uint32_t, int16_t> mEnchantDemonLookup;

  /// Map of enchantment IDs by item ID
  std::unordered_map<uint32_t, int16_t> mEnchantItemLookup;

  /// Map of equipment set information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiEquipmentSetData>>
      mEquipmentSetData;

  /// Map of equipment IDs to set IDs they belong to
  std::unordered_map<uint32_t, std::list<uint32_t>> mEquipmentSetLookup;

  /// Map of item exchange data by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiExchangeData>>
      mExchangeData;

  /// Map of character expertise information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiExpertData>>
      mExpertData;

  /// Map of demon race IDs to maximum fusion range levels paired
  /// with their corresponding result demon
  std::unordered_map<uint8_t, std::list<std::pair<uint8_t, uint32_t>>>
      mFusionRanges;

  /// Map of digitalize "guardian assist" information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiGuardianAssistData>>
      mGuardianAssistData;

  /// Map of digitalize "guardian level" information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiGuardianLevelData>>
      mGuardianLevelData;

  /// Map of digitalize "guardian special" information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiGuardianSpecialData>>
      mGuardianSpecialData;

  /// Map of digitalize "guardian unlock" information by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiGuardianUnlockData>>
      mGuardianUnlockData;

  /// Map of human NPC definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiHNPCData>> mHNPCData;

  /// Map of item definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiItemData>> mItemData;

  /// Map of mission definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiMissionData>>
      mMissionData;

  /// Map of mitama reunion bonus definitions by ID
  std::unordered_map<uint32_t,
                     std::shared_ptr<objects::MiMitamaReunionBonusData>>
      mMitamaReunionBonusData;

  /// Map of mitama reunion set bonus definitions by ID
  std::unordered_map<uint32_t,
                     std::shared_ptr<objects::MiMitamaReunionSetBonusData>>
      mMitamaReunionSetBonusData;

  /// Map of mitama union bonus definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiMitamaUnionBonusData>>
      mMitamaUnionBonusData;

  /// Map of item modification definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiModificationData>>
      mModificationData;

  /// Map of item modification definition IDs by item ID
  std::unordered_map<uint32_t, uint32_t> mModificationLookup;

  /// Map of item modification extra effect definitions by their group ID,
  /// slot then sub ID. Because there are no unique IDs for this object, this is
  /// how the data must be identified.
  std::unordered_map<
      uint8_t,
      std::unordered_map<
          uint8_t,
          std::unordered_map<
              uint16_t, std::shared_ptr<objects::MiModificationExtEffectData>>>>
      mModificationExtEffectData;

  /// Map of item modification extra recipe definitions by ID
  std::unordered_map<uint32_t,
                     std::shared_ptr<objects::MiModificationExtRecipeData>>
      mModificationExtRecipeData;

  /// Map of item modification extra recipe definition IDs by item ID
  std::unordered_map<uint32_t, uint32_t> mModificationExtRecipeLookup;

  /// Map of item modification trigger definitions by ID
  std::unordered_map<uint16_t,
                     std::shared_ptr<objects::MiModificationTriggerData>>
      mModificationTriggerData;

  /// Map of item modification effect definitions by ID
  std::unordered_map<uint16_t, std::shared_ptr<objects::MiModifiedEffectData>>
      mModifiedEffectData;

  /// Map of NPC barter definitions by ID
  std::unordered_map<uint16_t, std::shared_ptr<objects::MiNPCBarterData>>
      mNPCBarterData;

  /// Map of NPC barter condition definitions by ID
  std::unordered_map<uint16_t,
                     std::shared_ptr<objects::MiNPCBarterConditionData>>
      mNPCBarterConditionData;

  /// Map of NPC barter group definitions by ID
  std::unordered_map<uint16_t, std::shared_ptr<objects::MiNPCBarterGroupData>>
      mNPCBarterGroupData;

  /// Map of server object NPC definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiONPCData>> mONPCData;

  /// Map of quest bonus code definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiQuestBonusCodeData>>
      mQuestBonusCodeData;

  /// Map of quest definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiQuestData>>
      mQuestData;

  /// Map of shop product definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiShopProductData>>
      mShopProductData;

  /// Map of s-item tokusei IDs by item ID
  std::unordered_map<uint32_t, std::set<int32_t>> mSItemTokusei;

  /// Map of skill definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiSkillData>>
      mSkillData;

  /// Map of skill function IDs to skill IDs
  std::unordered_map<uint16_t, std::set<uint32_t>> mFunctionIDSkills;

  /// Map of filename to map of spots by ID
  std::unordered_map<
      std::string,
      std::unordered_map<uint32_t, std::shared_ptr<objects::MiSpotData>>>
      mSpotData;

  /// Map of s-status definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiSStatusData>>
      mSStatusData;

  /// Map of status definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiStatusData>>
      mStatusData;

  /// Map of synthesis definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiSynthesisData>>
      mSynthesisData;

  /// Map of material tank definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiTankData>> mTankData;

  /// Map of time limit definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiTimeLimitData>>
      mTimeLimitData;

  /// Map of title definitions by ID
  std::unordered_map<int16_t, std::shared_ptr<objects::MiTitleData>> mTitleData;

  /// Set of all (non-special) title IDs
  std::set<int16_t> mTitleIDs;

  /// Map of special fusion definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiTriUnionSpecialData>>
      mTriUnionSpecialData;

  /// Map of source demon IDs to special fusions they belong to
  std::unordered_map<uint32_t, std::list<uint32_t>>
      mTriUnionSpecialDataBySourceID;

  /// Map of (Diaspora) ura field tower definitions by dungeon ID, then ID
  std::unordered_map<
      uint32_t, std::unordered_map<
                    uint32_t, std::shared_ptr<objects::MiUraFieldTowerData>>>
      mUraFieldTowerData;

  /// Map of warp point definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiWarpPointData>>
      mWarpPointData;

  /// Map of zone definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiZoneData>> mZoneData;

  /// Map of enchant set definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::EnchantSetData>>
      mEnchantSetData;

  /// Map of enchant set definition IDs by effect ID
  std::unordered_map<int16_t, std::list<uint32_t>> mEnchantSetLookup;

  /// Map of enchant special definitions by ID
  std::unordered_map<uint32_t, std::shared_ptr<objects::EnchantSpecialData>>
      mEnchantSpecialData;

  /// Map of enchant special definition IDs by input item ID
  std::unordered_map<uint32_t, std::list<uint32_t>> mEnchantSpecialLookup;

  /// Map of tokusei definitions by ID
  std::unordered_map<int32_t, std::shared_ptr<objects::Tokusei>> mTokuseiData;
};

}  // namespace libcomp

#endif  // LIBCOMP_SRC_DEFINITIONMANAGER_H
