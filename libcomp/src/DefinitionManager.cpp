/**
 * @file libcomp/src/DefinitionManager.cpp
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

#include "DefinitionManager.h"

// libcomp Includes
#include "Constants.h"
#include "Log.h"

#ifndef EXOTIC_PLATFORM
#include "ScriptEngine.h"
#endif  // !EXOTIC_PLATFORM

// object Includes
#include <EnchantSetData.h>
#include <EnchantSpecialData.h>
#include <MiAIData.h>
#include <MiBlendData.h>
#include <MiBlendExtData.h>
#include <MiCHouraiData.h>
#include <MiCItemBaseData.h>
#include <MiCItemData.h>
#include <MiCultureItemData.h>
#include <MiDCategoryData.h>
#include <MiDamageData.h>
#include <MiDevilBookData.h>
#include <MiDevilBoostData.h>
#include <MiDevilBoostExtraData.h>
#include <MiDevilBoostItemData.h>
#include <MiDevilBoostLotData.h>
#include <MiDevilCrystalData.h>
#include <MiDevilData.h>
#include <MiDevilEquipmentData.h>
#include <MiDevilEquipmentItemData.h>
#include <MiDevilFusionData.h>
#include <MiDevilLVUpRateData.h>
#include <MiDisassemblyData.h>
#include <MiDisassemblyTriggerData.h>
#include <MiDynamicMapData.h>
#include <MiEnchantData.h>
#include <MiEquipmentSetData.h>
#include <MiExchangeData.h>
#include <MiExpertClassData.h>
#include <MiExpertData.h>
#include <MiExpertRankData.h>
#include <MiGrowthData.h>
#include <MiGuardianAssistData.h>
#include <MiGuardianLevelData.h>
#include <MiGuardianLevelDataEntry.h>
#include <MiGuardianSpecialData.h>
#include <MiGuardianUnlockData.h>
#include <MiHNPCBasicData.h>
#include <MiHNPCData.h>
#include <MiItemData.h>
#include <MiMissionData.h>
#include <MiMitamaReunionBonusData.h>
#include <MiMitamaReunionSetBonusData.h>
#include <MiMitamaUnionBonusData.h>
#include <MiModificationData.h>
#include <MiModificationExtEffectData.h>
#include <MiModificationExtRecipeData.h>
#include <MiModificationTriggerData.h>
#include <MiModifiedEffectData.h>
#include <MiNPCBarterConditionData.h>
#include <MiNPCBarterData.h>
#include <MiNPCBarterGroupData.h>
#include <MiNPCBasicData.h>
#include <MiONPCData.h>
#include <MiQuestBonusCodeData.h>
#include <MiQuestData.h>
#include <MiSItemData.h>
#include <MiSStatusData.h>
#include <MiShopProductData.h>
#include <MiSkillData.h>
#include <MiSkillItemStatusCommonData.h>
#include <MiSpotData.h>
#include <MiStatusData.h>
#include <MiSynthesisData.h>
#include <MiTankData.h>
#include <MiTimeLimitData.h>
#include <MiTitleData.h>
#include <MiTriUnionSpecialData.h>
#include <MiUnionData.h>
#include <MiUraFieldTowerData.h>
#include <MiWarpPointData.h>
#include <MiZoneBasicData.h>
#include <MiZoneData.h>
#include <QmpFile.h>
#include <Tokusei.h>

using namespace libcomp;

DefinitionManager::DefinitionManager() {}

DefinitionManager::~DefinitionManager() {}

const std::shared_ptr<objects::MiAIData> DefinitionManager::GetAIData(
    uint32_t id) {
  return GetRecordByID(id, mAIData);
}

const std::shared_ptr<objects::MiBlendData> DefinitionManager::GetBlendData(
    uint32_t id) {
  return GetRecordByID(id, mBlendData);
}

const std::shared_ptr<objects::MiBlendExtData>
DefinitionManager::GetBlendExtData(uint32_t id) {
  return GetRecordByID(id, mBlendExtData);
}

std::unordered_map<int8_t, std::shared_ptr<objects::MiCHouraiData>>
DefinitionManager::GetCHouraiData() {
  return mCHouraiData;
}

const std::shared_ptr<objects::MiCultureItemData>
DefinitionManager::GetCultureItemData(uint32_t id) {
  return GetRecordByID(id, mCultureItemData);
}

const std::shared_ptr<objects::MiDevilBookData>
DefinitionManager::GetDevilBookData(uint32_t id) {
  return GetRecordByID(id, mDevilBookData);
}

std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilBookData>>
DefinitionManager::GetDevilBookData() {
  return mDevilBookData;
}

const std::shared_ptr<objects::MiDevilBoostData>
DefinitionManager::GetDevilBoostData(uint32_t id) {
  return GetRecordByID(id, mDevilBoostData);
}

const std::shared_ptr<objects::MiDevilBoostExtraData>
DefinitionManager::GetDevilBoostExtraData(uint16_t id) {
  return GetRecordByID(id, mDevilBoostExtraData);
}

const std::shared_ptr<objects::MiDevilBoostItemData>
DefinitionManager::GetDevilBoostItemData(uint32_t id) {
  return GetRecordByID(id, mDevilBoostItemData);
}

std::list<uint16_t> DefinitionManager::GetDevilBoostLotIDs(int32_t count) {
  std::list<uint16_t> results;

  bool directFound = false;
  for (auto &pair : mDevilBoostLots) {
    directFound |= pair.first == count;

    if ((count < 100 || pair.first % 100 == 0) &&
        (pair.first == count || (count % pair.first) == 0)) {
      for (uint16_t stackID : pair.second) {
        results.push_back(stackID);
      }
    }
  }

  // For the lower values, only include if a direct match is found
  if (count < 100 && !directFound) {
    results.clear();
  }

  return results;
}

std::shared_ptr<objects::MiDevilData> DefinitionManager::GetDevilData(
    uint32_t id) {
  return GetRecordByID(id, mDevilData);
}

const std::shared_ptr<objects::MiDevilData> DefinitionManager::GetDevilData(
    const libcomp::String &name) {
  auto iter = mDevilNameLookup.find(name);
  if (iter != mDevilNameLookup.end()) {
    return GetDevilData(iter->second);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiDevilEquipmentData>
DefinitionManager::GetDevilEquipmentData(uint32_t id) {
  return GetRecordByID(id, mDevilEquipmentData);
}

const std::shared_ptr<objects::MiDevilEquipmentItemData>
DefinitionManager::GetDevilEquipmentItemData(uint32_t id) {
  return GetRecordByID(id, mDevilEquipmentItemData);
}

const std::shared_ptr<objects::MiDevilFusionData>
DefinitionManager::GetDevilFusionData(uint32_t id) {
  return GetRecordByID(id, mDevilFusionData);
}

std::set<uint32_t> DefinitionManager::GetDevilFusionIDsByDemonID(
    uint32_t demonID) {
  std::set<uint32_t> results;

  // Results are gathered based on direct and base demon ID
  auto demonDef = GetDevilData(demonID);
  uint32_t baseDemonID =
      demonDef ? demonDef->GetUnionData()->GetBaseDemonID() : 0;
  for (uint32_t id : {demonID, baseDemonID}) {
    auto iter = mDevilFusionLookup.find(id);
    if (iter != mDevilFusionLookup.end()) {
      for (uint32_t fusionID : iter->second) {
        results.insert(fusionID);
      }
    }
  }

  return results;
}

const std::shared_ptr<objects::MiDevilLVUpRateData>
DefinitionManager::GetDevilLVUpRateData(uint32_t id) {
  return GetRecordByID(id, mDevilLVUpRateData);
}

std::unordered_map<uint32_t, std::shared_ptr<objects::MiDevilLVUpRateData>>
DefinitionManager::GetAllDevilLVUpRateData() {
  return mDevilLVUpRateData;
}

const std::shared_ptr<objects::MiDisassemblyData>
DefinitionManager::GetDisassemblyData(uint32_t id) {
  return GetRecordByID(id, mDisassemblyData);
}

const std::shared_ptr<objects::MiDisassemblyData>
DefinitionManager::GetDisassemblyDataByItemID(uint32_t itemID) {
  auto iter = mDisassemblyLookup.find(itemID);
  if (iter != mDisassemblyLookup.end()) {
    return GetRecordByID(iter->second, mDisassemblyData);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiDisassemblyTriggerData>
DefinitionManager::GetDisassemblyTriggerData(uint32_t id) {
  return GetRecordByID(id, mDisassemblyTriggerData);
}

const std::list<uint32_t> DefinitionManager::GetDisassembledItemIDs() {
  return mDisassembledItemIDs;
}

const std::shared_ptr<objects::MiDynamicMapData>
DefinitionManager::GetDynamicMapData(uint32_t id) {
  return GetRecordByID(id, mDynamicMapData);
}

const std::shared_ptr<objects::MiEnchantData> DefinitionManager::GetEnchantData(
    int16_t id) {
  return GetRecordByID(id, mEnchantData);
}

std::unordered_map<int16_t, std::shared_ptr<objects::MiEnchantData>>
DefinitionManager::GetAllEnchantData() {
  return mEnchantData;
}

const std::shared_ptr<objects::MiEnchantData>
DefinitionManager::GetEnchantDataByDemonID(uint32_t demonID) {
  auto iter = mEnchantDemonLookup.find(demonID);
  if (iter != mEnchantDemonLookup.end()) {
    return GetEnchantData(iter->second);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiEnchantData>
DefinitionManager::GetEnchantDataByItemID(uint32_t itemID) {
  auto iter = mEnchantItemLookup.find(itemID);
  if (iter != mEnchantItemLookup.end()) {
    return GetEnchantData(iter->second);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiEquipmentSetData>
DefinitionManager::GetEquipmentSetData(uint32_t id) {
  return GetRecordByID(id, mEquipmentSetData);
}

std::list<std::shared_ptr<objects::MiEquipmentSetData>>
DefinitionManager::GetEquipmentSetDataByItem(uint32_t equipmentID) {
  std::list<std::shared_ptr<objects::MiEquipmentSetData>> retval;

  auto it = mEquipmentSetLookup.find(equipmentID);
  if (it != mEquipmentSetLookup.end()) {
    for (auto setID : it->second) {
      auto equipmentSet = GetEquipmentSetData(setID);
      if (equipmentSet) {
        retval.push_back(equipmentSet);
      }
    }
  }

  return retval;
}

const std::shared_ptr<objects::MiExchangeData>
DefinitionManager::GetExchangeData(uint32_t id) {
  return GetRecordByID(id, mExchangeData);
}

const std::shared_ptr<objects::MiExpertData>
DefinitionManager::GetExpertClassData(uint32_t id) {
  return GetRecordByID(id, mExpertData);
}

const std::list<std::pair<uint8_t, uint32_t>>
DefinitionManager::GetFusionRanges(uint8_t raceID) {
  auto iter = mFusionRanges.find(raceID);
  if (iter != mFusionRanges.end()) {
    return iter->second;
  }

  return std::list<std::pair<uint8_t, uint32_t>>();
}

const std::shared_ptr<objects::MiGuardianAssistData>
DefinitionManager::GetGuardianAssistData(uint32_t id) {
  return GetRecordByID(id, mGuardianAssistData);
}

const std::set<uint8_t> DefinitionManager::GetGuardianRaceIDs() {
  std::set<uint8_t> raceIDs;
  for (auto &pair : mGuardianLevelData) {
    // "Level 0" entry uses XP field as an enabled flag
    if (pair.second->GetLevels(0)->GetNextXP() > 0) {
      raceIDs.insert((uint8_t)pair.first);
    }
  }

  return raceIDs;
}

const std::shared_ptr<objects::MiGuardianLevelData>
DefinitionManager::GetGuardianLevelData(uint32_t id) {
  return GetRecordByID(id, mGuardianLevelData);
}

const std::shared_ptr<objects::MiGuardianSpecialData>
DefinitionManager::GetGuardianSpecialData(uint32_t id) {
  return GetRecordByID(id, mGuardianSpecialData);
}

const std::shared_ptr<objects::MiGuardianUnlockData>
DefinitionManager::GetGuardianUnlockData(uint32_t id) {
  return GetRecordByID(id, mGuardianUnlockData);
}

const std::shared_ptr<objects::MiHNPCData> DefinitionManager::GetHNPCData(
    uint32_t id) {
  return GetRecordByID(id, mHNPCData);
}

const std::shared_ptr<objects::MiItemData> DefinitionManager::GetItemData(
    uint32_t id) {
  return GetRecordByID(id, mItemData);
}

const std::shared_ptr<objects::MiMissionData> DefinitionManager::GetMissionData(
    uint32_t id) {
  return GetRecordByID(id, mMissionData);
}

const std::shared_ptr<objects::MiMitamaReunionBonusData>
DefinitionManager::GetMitamaReunionBonusData(uint32_t id) {
  return GetRecordByID(id, mMitamaReunionBonusData);
}

std::unordered_map<uint32_t,
                   std::shared_ptr<objects::MiMitamaReunionSetBonusData>>
DefinitionManager::GetMitamaReunionSetBonusData() {
  return mMitamaReunionSetBonusData;
}

const std::shared_ptr<objects::MiMitamaUnionBonusData>
DefinitionManager::GetMitamaUnionBonusData(uint32_t id) {
  return GetRecordByID(id, mMitamaUnionBonusData);
}

const std::shared_ptr<objects::MiModificationData>
DefinitionManager::GetModificationData(uint32_t id) {
  return GetRecordByID(id, mModificationData);
}

const std::shared_ptr<objects::MiModificationData>
DefinitionManager::GetModificationDataByItemID(uint32_t itemID) {
  auto iter = mModificationLookup.find(itemID);
  if (iter != mModificationLookup.end()) {
    return GetRecordByID(iter->second, mModificationData);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiModificationExtEffectData>
DefinitionManager::GetModificationExtEffectData(uint8_t groupID, uint8_t slot,
                                                uint16_t subID) {
  auto iter1 = mModificationExtEffectData.find(groupID);
  if (iter1 != mModificationExtEffectData.end()) {
    auto iter2 = iter1->second.find(slot);
    if (iter2 != iter1->second.end()) {
      return GetRecordByID(subID, iter2->second);
    }
  }

  return nullptr;
}

const std::shared_ptr<objects::MiModificationExtRecipeData>
DefinitionManager::GetModificationExtRecipeData(uint32_t id) {
  return GetRecordByID(id, mModificationExtRecipeData);
}

const std::shared_ptr<objects::MiModificationExtRecipeData>
DefinitionManager::GetModificationExtRecipeDataByItemID(uint32_t itemID) {
  auto iter = mModificationExtRecipeLookup.find(itemID);
  if (iter != mModificationExtRecipeLookup.end()) {
    return GetRecordByID(iter->second, mModificationExtRecipeData);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiModificationTriggerData>
DefinitionManager::GetModificationTriggerData(uint16_t id) {
  return GetRecordByID(id, mModificationTriggerData);
}

const std::shared_ptr<objects::MiModifiedEffectData>
DefinitionManager::GetModifiedEffectData(uint16_t id) {
  return GetRecordByID(id, mModifiedEffectData);
}

const std::shared_ptr<objects::MiItemData> DefinitionManager::GetItemData(
    const libcomp::String &name) {
  auto iter = mCItemNameLookup.find(name);
  if (iter != mCItemNameLookup.end()) {
    return GetRecordByID(iter->second, mItemData);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiNPCBarterData>
DefinitionManager::GetNPCBarterData(uint16_t id) {
  return GetRecordByID(id, mNPCBarterData);
}

const std::shared_ptr<objects::MiNPCBarterConditionData>
DefinitionManager::GetNPCBarterConditionData(uint16_t id) {
  return GetRecordByID(id, mNPCBarterConditionData);
}

const std::shared_ptr<objects::MiNPCBarterGroupData>
DefinitionManager::GetNPCBarterGroupData(uint16_t id) {
  return GetRecordByID(id, mNPCBarterGroupData);
}

const std::shared_ptr<objects::MiONPCData> DefinitionManager::GetONPCData(
    uint32_t id) {
  return GetRecordByID(id, mONPCData);
}

const std::shared_ptr<objects::MiQuestBonusCodeData>
DefinitionManager::GetQuestBonusCodeData(uint32_t id) {
  return GetRecordByID(id, mQuestBonusCodeData);
}

const std::shared_ptr<objects::MiQuestData> DefinitionManager::GetQuestData(
    uint32_t id) {
  return GetRecordByID(id, mQuestData);
}

const std::shared_ptr<objects::MiShopProductData>
DefinitionManager::GetShopProductData(uint32_t id) {
  return GetRecordByID(id, mShopProductData);
}

std::set<int32_t> DefinitionManager::GetSItemTokusei(uint32_t id) {
  auto it = mSItemTokusei.find(id);
  return it != mSItemTokusei.end() ? it->second : std::set<int32_t>();
}

const std::shared_ptr<objects::MiSkillData> DefinitionManager::GetSkillData(
    uint32_t id) {
  return GetRecordByID(id, mSkillData);
}

std::set<uint32_t> DefinitionManager::GetFunctionIDSkills(uint16_t fid) const {
  auto it = mFunctionIDSkills.find(fid);
  return it != mFunctionIDSkills.end() ? it->second : std::set<uint32_t>();
}

const std::unordered_map<uint32_t, std::shared_ptr<objects::MiSpotData>>
DefinitionManager::GetSpotData(uint32_t dynamicMapID) {
  std::unordered_map<uint32_t, std::shared_ptr<objects::MiSpotData>> result;

  auto dynamicMap = GetDynamicMapData(dynamicMapID);
  if (dynamicMap) {
    std::string filename(dynamicMap->GetSpotDataFile().C());

    auto it = mSpotData.find(filename);
    if (it != mSpotData.end()) {
      result = it->second;
    }
  }

  return result;
}

const std::shared_ptr<objects::MiSStatusData> DefinitionManager::GetSStatusData(
    uint32_t id) {
  return GetRecordByID(id, mSStatusData);
}

const std::shared_ptr<objects::MiStatusData> DefinitionManager::GetStatusData(
    uint32_t id) {
  return GetRecordByID(id, mStatusData);
}

const std::shared_ptr<objects::MiSynthesisData>
DefinitionManager::GetSynthesisData(uint32_t id) {
  return GetRecordByID(id, mSynthesisData);
}

std::unordered_map<uint32_t, std::shared_ptr<objects::MiSynthesisData>>
DefinitionManager::GetAllSynthesisData() {
  return mSynthesisData;
}

std::unordered_map<uint32_t, std::shared_ptr<objects::MiTankData>>
DefinitionManager::GetTankData() {
  return mTankData;
}

const std::shared_ptr<objects::MiTimeLimitData>
DefinitionManager::GetTimeLimitData(uint32_t id) {
  return GetRecordByID(id, mTimeLimitData);
}

const std::shared_ptr<objects::MiTitleData> DefinitionManager::GetTitleData(
    int16_t id) {
  return GetRecordByID(id, mTitleData);
}

std::set<int16_t> DefinitionManager::GetTitleIDs() { return mTitleIDs; }

const std::list<std::shared_ptr<objects::MiTriUnionSpecialData>>
DefinitionManager::GetTriUnionSpecialData(uint32_t sourceDemonTypeID) {
  std::list<std::shared_ptr<objects::MiTriUnionSpecialData>> result;
  auto it = mTriUnionSpecialDataBySourceID.find(sourceDemonTypeID);
  if (it != mTriUnionSpecialDataBySourceID.end()) {
    for (auto specialID : it->second) {
      result.push_back(mTriUnionSpecialData[specialID]);
    }
  }

  // Gather additional fusions from base demon ID in case a variant fusion
  // is being performed
  auto demonDef = GetDevilData(sourceDemonTypeID);
  uint32_t sourceBaseDemonTypeID =
      demonDef ? demonDef->GetUnionData()->GetBaseDemonID() : 0;
  if (sourceBaseDemonTypeID) {
    it = mTriUnionSpecialDataBySourceID.find(sourceBaseDemonTypeID);
    if (it != mTriUnionSpecialDataBySourceID.end()) {
      for (auto specialID : it->second) {
        result.push_back(mTriUnionSpecialData[specialID]);
      }
    }
  }

  result.unique();

  return result;
}

const std::shared_ptr<objects::MiUraFieldTowerData>
DefinitionManager::GetUraFieldTowerData(uint32_t dungeonID, uint32_t id) {
  auto it = mUraFieldTowerData.find(dungeonID);
  if (it != mUraFieldTowerData.end()) {
    return GetRecordByID(id, it->second);
  }

  return nullptr;
}

const std::shared_ptr<objects::MiWarpPointData>
DefinitionManager::GetWarpPointData(uint32_t id) {
  return GetRecordByID(id, mWarpPointData);
}

const std::shared_ptr<objects::MiZoneData> DefinitionManager::GetZoneData(
    uint32_t id) {
  return GetRecordByID(id, mZoneData);
}

const std::shared_ptr<objects::EnchantSetData>
DefinitionManager::GetEnchantSetData(uint32_t id) {
  return GetRecordByID(id, mEnchantSetData);
}

std::list<std::shared_ptr<objects::EnchantSetData>>
DefinitionManager::GetEnchantSetDataByEffect(int16_t effectID) {
  std::list<std::shared_ptr<objects::EnchantSetData>> retval;

  auto it = mEnchantSetLookup.find(effectID);
  if (it != mEnchantSetLookup.end()) {
    for (auto setID : it->second) {
      auto enchantSet = GetEnchantSetData(setID);
      if (enchantSet) {
        retval.push_back(enchantSet);
      }
    }
  }

  return retval;
}

const std::unordered_map<uint32_t, std::shared_ptr<objects::EnchantSetData>>
DefinitionManager::GetAllEnchantSetData() {
  return mEnchantSetData;
}

const std::shared_ptr<objects::EnchantSpecialData>
DefinitionManager::GetEnchantSpecialData(uint32_t id) {
  return GetRecordByID(id, mEnchantSpecialData);
}

std::list<std::shared_ptr<objects::EnchantSpecialData>>
DefinitionManager::GetEnchantSpecialDataByInputItem(uint32_t itemID) {
  std::list<std::shared_ptr<objects::EnchantSpecialData>> retval;

  auto it = mEnchantSpecialLookup.find(itemID);
  if (it != mEnchantSpecialLookup.end()) {
    for (auto specialID : it->second) {
      auto specialSet = GetEnchantSpecialData(specialID);
      if (specialSet) {
        retval.push_back(specialSet);
      }
    }
  }

  return retval;
}

const std::shared_ptr<objects::Tokusei> DefinitionManager::GetTokuseiData(
    int32_t id) {
  return GetRecordByID(id, mTokuseiData);
}

const std::unordered_map<int32_t, std::shared_ptr<objects::Tokusei>>
DefinitionManager::GetAllTokuseiData() {
  return mTokuseiData;
}

namespace libcomp {
template <>
bool DefinitionManager::LoadData<objects::MiAIData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiAIData>> records;
  bool success = LoadBinaryData<objects::MiAIData>(
      pDataStore, "Shield/AIData.sbin", true, 0, records);
  for (auto record : records) {
    mAIData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiBlendData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiBlendData>> records;
  bool success = LoadBinaryData<objects::MiBlendData>(
      pDataStore, "Shield/BlendData.sbin", true, 0, records);
  for (auto record : records) {
    mBlendData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiBlendExtData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiBlendExtData>> records;
  bool success = LoadBinaryData<objects::MiBlendExtData>(
      pDataStore, "Shield/BlendExtData.sbin", true, 0, records);
  for (auto record : records) {
    mBlendExtData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiCHouraiData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiCHouraiData>> records;
  bool success = LoadBinaryData<objects::MiCHouraiData>(
      pDataStore, "Shield/CHouraiData.sbin", true, 0, records);
  for (auto record : records) {
    mCHouraiData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiCItemData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiCItemData>> records;
  bool success = LoadBinaryData<objects::MiCItemData>(
      pDataStore, "Shield/CItemData.sbin", true, 0, records);
  for (auto record : records) {
    auto id = record->GetBaseData()->GetID();
    auto name = record->GetBaseData()->GetName();
    if (mCItemNameLookup.find(name) == mCItemNameLookup.end()) {
      mCItemNameLookup[name] = id;
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiCultureItemData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiCultureItemData>> records;
  bool success = LoadBinaryData<objects::MiCultureItemData>(
      pDataStore, "Shield/CultureItemData.sbin", true, 0, records);
  for (auto record : records) {
    mCultureItemData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilBookData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilBookData>> records;
  bool success = LoadBinaryData<objects::MiDevilBookData>(
      pDataStore, "Shield/DevilBookData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilBookData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilBoostData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilBoostData>> records;
  bool success = LoadBinaryData<objects::MiDevilBoostData>(
      pDataStore, "Shield/DevilBoostData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilBoostData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilBoostExtraData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilBoostExtraData>> records;
  bool success = LoadBinaryData<objects::MiDevilBoostExtraData>(
      pDataStore, "Shield/DevilBoostExtraData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilBoostExtraData[record->GetStackID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilBoostItemData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilBoostItemData>> records;
  bool success = LoadBinaryData<objects::MiDevilBoostItemData>(
      pDataStore, "Shield/DevilBoostItemData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilBoostItemData[record->GetItemID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilBoostLotData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilBoostLotData>> records;
  bool success = LoadBinaryData<objects::MiDevilBoostLotData>(
      pDataStore, "Shield/DevilBoostLotData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilBoostLots[(int32_t)record->GetLot()].push_back(record->GetStackID());
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilData>> records;
  bool success = LoadBinaryData<objects::MiDevilData>(
      pDataStore, "Shield/DevilData.sbin", true, 0, records);
  for (auto record : records) {
    auto id = record->GetBasic()->GetID();
    auto name = record->GetBasic()->GetName();

    mDevilData[id] = record;
    if (mDevilNameLookup.find(name) == mDevilNameLookup.end()) {
      mDevilNameLookup[name] = id;
    }

    // If the fusion options contain 2-way fusion result, add to
    // the fusion range map
    auto fusionOptions = record->GetUnionData()->GetFusionOptions();
    if (fusionOptions & FUSION_OPTION_2WAY_RANGE) {
      mFusionRanges[(uint8_t)record->GetCategory()->GetRace()].push_back(
          std::pair<uint8_t, uint32_t>(
              (uint8_t)record->GetGrowth()->GetBaseLevel(), id));
    }
  }

  // Sort the fusion ranges
  for (auto &pair : mFusionRanges) {
    auto &ranges = pair.second;
    ranges.sort([](const std::pair<uint8_t, uint32_t> &a,
                   const std::pair<uint8_t, uint32_t> &b) {
      return a.first < b.first;
    });
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilEquipmentData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilEquipmentData>> records;
  bool success = LoadBinaryData<objects::MiDevilEquipmentData>(
      pDataStore, "Shield/DevilEquipmentData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilEquipmentData[record->GetSkillID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilEquipmentItemData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilEquipmentItemData>> records;
  bool success = LoadBinaryData<objects::MiDevilEquipmentItemData>(
      pDataStore, "Shield/DevilEquipmentItemData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilEquipmentItemData[record->GetItemID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilFusionData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilFusionData>> records;
  bool success = LoadBinaryData<objects::MiDevilFusionData>(
      pDataStore, "Shield/DevilFusionData.sbin", true, 0, records);
  for (auto record : records) {
    uint32_t skillID = record->GetSkillID();
    mDevilFusionData[skillID] = record;

    for (uint32_t demonID : record->GetRequiredDemons()) {
      if (demonID) {
        mDevilFusionLookup[demonID].insert(skillID);
      }
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDevilLVUpRateData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDevilLVUpRateData>> records;
  bool success = LoadBinaryData<objects::MiDevilLVUpRateData>(
      pDataStore, "Shield/DevilLVUpRateData.sbin", true, 0, records);
  for (auto record : records) {
    mDevilLVUpRateData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDisassemblyData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDisassemblyData>> records;
  bool success = LoadBinaryData<objects::MiDisassemblyData>(
      pDataStore, "Shield/DisassemblyData.sbin", true, 0, records);
  for (auto record : records) {
    uint32_t id = record->GetID();
    uint32_t itemID = record->GetItemID();

    mDisassemblyData[id] = record;

    if (mDisassemblyLookup.find(itemID) != mDisassemblyLookup.end()) {
      LogDefinitionManagerWarning([&]() {
        return libcomp::String(
                   "Duplicate item encountered"
                   " for disassembly mapping: %1\n")
            .Arg(itemID);
      });
    } else {
      mDisassemblyLookup[itemID] = id;
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDisassemblyTriggerData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDisassemblyTriggerData>> records;
  bool success = LoadBinaryData<objects::MiDisassemblyTriggerData>(
      pDataStore, "Shield/DisassemblyTriggerData.sbin", true, 0, records);
  for (auto record : records) {
    mDisassemblyTriggerData[record->GetID()] = record;
    mDisassembledItemIDs.push_back(record->GetID());
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiDynamicMapData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiDynamicMapData>> records;
  bool success = LoadBinaryData<objects::MiDynamicMapData>(
      pDataStore, "Client/DynamicMapData.bin", false, 0, records);

  uint16_t spotLoadCount = 0;
  for (auto record : records) {
    mDynamicMapData[record->GetID()] = record;

    std::string filename(record->GetSpotDataFile().C());
    if (filename.length() > 0 && mSpotData.find(filename) == mSpotData.end()) {
      auto path = libcomp::String("Client/%1").Arg(filename);

      std::list<std::shared_ptr<objects::MiSpotData>> spotRecords;
      bool spotSuccess = LoadBinaryData<objects::MiSpotData>(
          pDataStore, path, false, 0, spotRecords, false);

      // Dynamic map reference to an invalid file only becomes a
      // problem if a zone actually uses it
      if (!spotSuccess && !pDataStore->Open(path)) {
        LogDefinitionManagerWarning([filename]() {
          return libcomp::String(
                     "Invalid spot file listed in DynamicMapData: %1\n")
              .Arg(filename);
        });

        continue;
      }

      spotLoadCount++;

      if (spotSuccess) {
        for (auto spotRecord : spotRecords) {
          mSpotData[filename][spotRecord->GetID()] = spotRecord;
        }
      }
    }
  }

  if (spotLoadCount != (uint16_t)mSpotData.size()) {
    LogDefinitionManagerWarning([&]() {
      return libcomp::String("Loaded %1/%2 map spot definition files.\n")
          .Arg(mSpotData.size())
          .Arg(spotLoadCount);
    });
  } else {
    LogDefinitionManagerInfo([&]() {
      return libcomp::String("Loaded %1/%2 map spot definition files.\n")
          .Arg(spotLoadCount)
          .Arg(spotLoadCount);
    });
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiEnchantData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiEnchantData>> records;
  bool success = LoadBinaryData<objects::MiEnchantData>(
      pDataStore, "Shield/EnchantData.sbin", true, 0, records);
  for (auto record : records) {
    int16_t id = record->GetID();
    uint32_t demonID = record->GetDevilCrystal()->GetDemonID();
    uint32_t itemID = record->GetDevilCrystal()->GetItemID();

    mEnchantData[id] = record;

    if (demonID) {
      if (mEnchantDemonLookup.find(demonID) != mEnchantDemonLookup.end()) {
        LogDefinitionManagerWarning([&]() {
          return libcomp::String(
                     "Duplicate demon encountered for crystallization mapping: "
                     "%1\n")
              .Arg(demonID);
        });
      } else {
        mEnchantDemonLookup[demonID] = id;
      }
    }

    if (mEnchantItemLookup.find(itemID) != mEnchantItemLookup.end()) {
      LogDefinitionManagerWarning([&]() {
        return libcomp::String(
                   "Duplicate item encountered for crystallization mapping: "
                   "%1\n")
            .Arg(itemID);
      });
    } else {
      mEnchantItemLookup[itemID] = id;
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiEquipmentSetData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiEquipmentSetData>> records;
  bool success = LoadBinaryData<objects::MiEquipmentSetData>(
      pDataStore, "Shield/EquipmentSetData.sbin", true, 0, records);
  for (auto record : records) {
    bool equipmentFound = false;
    for (uint32_t equipmentID : record->GetEquipment()) {
      if (equipmentID) {
        mEquipmentSetLookup[equipmentID].push_back(record->GetID());
        equipmentFound = true;
      }
    }

    if (equipmentFound) {
      mEquipmentSetData[record->GetID()] = record;
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiExchangeData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiExchangeData>> records;
  bool success = LoadBinaryData<objects::MiExchangeData>(
      pDataStore, "Shield/ExchangeData.sbin", true, 0, records);
  for (auto record : records) {
    mExchangeData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiExpertData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiExpertData>> records;
  bool success = LoadBinaryData<objects::MiExpertData>(
      pDataStore, "Shield/ExpertClassData.sbin", true, 0, records);
  for (auto record : records) {
    mExpertData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiGuardianAssistData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiGuardianAssistData>> records;
  bool success = LoadBinaryData<objects::MiGuardianAssistData>(
      pDataStore, "Shield/GuardianAssistData.sbin", true, 0, records);
  for (auto record : records) {
    mGuardianAssistData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiGuardianLevelData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiGuardianLevelData>> records;
  bool success = LoadBinaryData<objects::MiGuardianLevelData>(
      pDataStore, "Shield/GuardianLevelData.sbin", true, 0, records);
  for (auto record : records) {
    mGuardianLevelData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiGuardianSpecialData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiGuardianSpecialData>> records;
  bool success = LoadBinaryData<objects::MiGuardianSpecialData>(
      pDataStore, "Shield/GuardianSpecialData.sbin", true, 0, records);
  for (auto record : records) {
    mGuardianSpecialData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiGuardianUnlockData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiGuardianUnlockData>> records;
  bool success = LoadBinaryData<objects::MiGuardianUnlockData>(
      pDataStore, "Shield/GuardianUnlockData.sbin", true, 0, records);
  for (auto record : records) {
    mGuardianUnlockData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiHNPCData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiHNPCData>> records;
  bool success = LoadBinaryData<objects::MiHNPCData>(
      pDataStore, "Shield/hNPCData.sbin", true, 0, records);
  for (auto record : records) {
    mHNPCData[record->GetBasic()->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiItemData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiItemData>> records;
  bool success = LoadBinaryData<objects::MiItemData>(
      pDataStore, "Shield/ItemData.sbin", true, 2, records);
  for (auto record : records) {
    mItemData[record->GetCommon()->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiMissionData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiMissionData>> records;
  bool success = LoadBinaryData<objects::MiMissionData>(
      pDataStore, "Shield/MissionData.sbin", true, 0, records);
  for (auto record : records) {
    mMissionData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiMitamaReunionBonusData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiMitamaReunionBonusData>> records;
  bool success = LoadBinaryData<objects::MiMitamaReunionBonusData>(
      pDataStore, "Shield/MitamaReunionBonusData.sbin", true, 0, records);
  for (auto record : records) {
    mMitamaReunionBonusData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiMitamaReunionSetBonusData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiMitamaReunionSetBonusData>> records;
  bool success = LoadBinaryData<objects::MiMitamaReunionSetBonusData>(
      pDataStore, "Shield/MitamaReunionSetBonusData.sbin", true, 0, records);
  for (auto record : records) {
    mMitamaReunionSetBonusData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiMitamaUnionBonusData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiMitamaUnionBonusData>> records;
  bool success = LoadBinaryData<objects::MiMitamaUnionBonusData>(
      pDataStore, "Shield/MitamaUnionBonusData.sbin", true, 0, records);
  for (auto record : records) {
    mMitamaUnionBonusData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiModificationData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiModificationData>> records;
  bool success = LoadBinaryData<objects::MiModificationData>(
      pDataStore, "Shield/ModificationData.sbin", true, 0, records);
  for (auto record : records) {
    uint32_t id = record->GetID();
    uint32_t itemID = record->GetItemID();

    mModificationData[id] = record;

    if (mModificationLookup.find(itemID) != mModificationLookup.end()) {
      LogDefinitionManagerWarning([&]() {
        return libcomp::String(
                   "Duplicate item encountered for modification mapping: %1\n")
            .Arg(itemID);
      });
    } else {
      mModificationLookup[itemID] = id;
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiModificationExtEffectData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiModificationExtEffectData>> records;
  bool success = LoadBinaryData<objects::MiModificationExtEffectData>(
      pDataStore, "Shield/ModificationExtEffectData.sbin", true, 0, records);
  for (auto record : records) {
    mModificationExtEffectData[record->GetGroupID()][record->GetSlot()]
                              [record->GetSubID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiModificationExtRecipeData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiModificationExtRecipeData>> records;
  bool success = LoadBinaryData<objects::MiModificationExtRecipeData>(
      pDataStore, "Shield/ModificationExtRecipeData.sbin", true, 0, records);
  for (auto record : records) {
    uint32_t itemID = record->GetItemID();
    if (itemID == static_cast<uint32_t>(-1)) continue;

    uint32_t id = record->GetID();
    mModificationExtRecipeData[id] = record;

    if (mModificationExtRecipeLookup.find(itemID) !=
        mModificationExtRecipeLookup.end()) {
      LogDefinitionManagerWarning([&]() {
        return libcomp::String(
                   "Duplicate item encountered for modification extra mapping: "
                   "%1\n")
            .Arg(itemID);
      });
    } else {
      mModificationExtRecipeLookup[itemID] = id;
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiModificationTriggerData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiModificationTriggerData>> records;
  bool success = LoadBinaryData<objects::MiModificationTriggerData>(
      pDataStore, "Shield/ModificationTriggerData.sbin", true, 0, records);
  for (auto record : records) {
    mModificationTriggerData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiModifiedEffectData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiModifiedEffectData>> records;
  bool success = LoadBinaryData<objects::MiModifiedEffectData>(
      pDataStore, "Shield/ModifiedEffectData.sbin", true, 0, records);
  for (auto record : records) {
    mModifiedEffectData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiNPCBarterData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiNPCBarterData>> records;
  bool success = LoadBinaryData<objects::MiNPCBarterData>(
      pDataStore, "Shield/NPCBarterData.sbin", true, 0, records);
  for (auto record : records) {
    mNPCBarterData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiNPCBarterConditionData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiNPCBarterConditionData>> records;
  bool success = LoadBinaryData<objects::MiNPCBarterConditionData>(
      pDataStore, "Shield/NPCBarterConditionData.sbin", true, 0, records);
  for (auto record : records) {
    mNPCBarterConditionData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiNPCBarterGroupData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiNPCBarterGroupData>> records;
  bool success = LoadBinaryData<objects::MiNPCBarterGroupData>(
      pDataStore, "Shield/NPCBarterGroupData.sbin", true, 0, records);
  for (auto record : records) {
    mNPCBarterGroupData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiONPCData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiONPCData>> records;
  bool success = LoadBinaryData<objects::MiONPCData>(
      pDataStore, "Shield/oNPCData.sbin", true, 0, records);
  for (auto record : records) {
    mONPCData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiQuestBonusCodeData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiQuestBonusCodeData>> records;
  bool success = LoadBinaryData<objects::MiQuestBonusCodeData>(
      pDataStore, "Shield/QuestBonusCodeData.sbin", true, 0, records);
  for (auto record : records) {
    mQuestBonusCodeData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiQuestData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiQuestData>> records;
  bool success = LoadBinaryData<objects::MiQuestData>(
      pDataStore, "Shield/QuestData.sbin", true, 0, records);
  for (auto record : records) {
    mQuestData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiShopProductData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiShopProductData>> records;
  bool success = LoadBinaryData<objects::MiShopProductData>(
      pDataStore, "Shield/ShopProductData.sbin", true, 0, records);
  for (auto record : records) {
    mShopProductData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiSItemData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiSItemData>> records;
  bool success = LoadBinaryData<objects::MiSItemData>(
      pDataStore, "Shield/SItemData.sbin", true, 0, records);
  for (auto record : records) {
    uint32_t itemID = record->GetID();
    for (auto tokuseiID : record->GetTokusei()) {
      if (tokuseiID != 0) {
        mSItemTokusei[itemID].insert(tokuseiID);
      }
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiSkillData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiSkillData>> records;
  bool success = LoadBinaryData<objects::MiSkillData>(
      pDataStore, "Shield/SkillData.sbin", true, 4, records);
  for (auto record : records) {
    uint32_t id = record->GetCommon()->GetID();
    uint16_t fid = record->GetDamage()->GetFunctionID();

    mSkillData[id] = record;

    if (fid) {
      mFunctionIDSkills[fid].insert(id);
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiStatusData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiStatusData>> records;
  bool success = LoadBinaryData<objects::MiStatusData>(
      pDataStore, "Shield/StatusData.sbin", true, 1, records);
  for (auto record : records) {
    mStatusData[record->GetCommon()->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiSynthesisData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiSynthesisData>> records;
  bool success = LoadBinaryData<objects::MiSynthesisData>(
      pDataStore, "Shield/SynthesisData.sbin", true, 0, records);
  for (auto record : records) {
    mSynthesisData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiTankData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiTankData>> records;
  bool success = LoadBinaryData<objects::MiTankData>(
      pDataStore, "Shield/TankData.sbin", true, 0, records);
  for (auto record : records) {
    mTankData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiTimeLimitData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiTimeLimitData>> records;
  bool success = LoadBinaryData<objects::MiTimeLimitData>(
      pDataStore, "Shield/TimeLimitData.sbin", true, 0, records);
  for (auto record : records) {
    mTimeLimitData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiTitleData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiTitleData>> records;
  bool success = LoadBinaryData<objects::MiTitleData>(
      pDataStore, "Shield/CodeNameData.sbin", true, 0, records);
  for (auto record : records) {
    int16_t id = record->GetID();

    mTitleData[id] = record;

    if (id >= MAX_SPECIAL_TITLE && !record->GetTitle().IsEmpty()) {
      mTitleIDs.insert(id);
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiTriUnionSpecialData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiTriUnionSpecialData>> records;
  bool success = LoadBinaryData<objects::MiTriUnionSpecialData>(
      pDataStore, "Shield/TriUnionSpecialData.sbin", true, 0, records);
  for (auto record : records) {
    auto id = record->GetID();
    mTriUnionSpecialData[id] = record;

    for (auto sourceID : {record->GetSourceID1(), record->GetSourceID2(),
                          record->GetSourceID3()}) {
      if (sourceID) {
        mTriUnionSpecialDataBySourceID[sourceID].push_back(id);
      }
    }
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiUraFieldTowerData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiUraFieldTowerData>> records;
  bool success = LoadBinaryData<objects::MiUraFieldTowerData>(
      pDataStore, "Shield/UraFieldTowerData.sbin", true, 0, records);
  for (auto record : records) {
    mUraFieldTowerData[record->GetDungeonID()][record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiWarpPointData>(
    DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiWarpPointData>> records;
  bool success = LoadBinaryData<objects::MiWarpPointData>(
      pDataStore, "Shield/WarpPointData.sbin", true, 0, records);
  for (auto record : records) {
    mWarpPointData[record->GetID()] = record;
  }

  return success;
}

template <>
bool DefinitionManager::LoadData<objects::MiZoneData>(DataStore *pDataStore) {
  std::list<std::shared_ptr<objects::MiZoneData>> records;
  bool success = LoadBinaryData<objects::MiZoneData>(
      pDataStore, "Shield/ZoneData.sbin", true, 0, records);
  for (auto record : records) {
    mZoneData[record->GetBasic()->GetID()] = record;
  }

  return success;
}
}  // namespace libcomp

bool DefinitionManager::LoadAllData(DataStore *pDataStore) {
  LogDefinitionManagerInfoMsg("Loading binary data definitions...\n");

  bool success = true;
  success &= LoadData<objects::MiAIData>(pDataStore);
  success &= LoadData<objects::MiBlendData>(pDataStore);
  success &= LoadData<objects::MiBlendExtData>(pDataStore);
  success &= LoadData<objects::MiCHouraiData>(pDataStore);
  success &= LoadData<objects::MiCItemData>(pDataStore);
  success &= LoadData<objects::MiCultureItemData>(pDataStore);
  success &= LoadData<objects::MiDevilData>(pDataStore);
  success &= LoadData<objects::MiDevilBookData>(pDataStore);
  success &= LoadData<objects::MiDevilBoostData>(pDataStore);
  success &= LoadData<objects::MiDevilBoostExtraData>(pDataStore);
  success &= LoadData<objects::MiDevilBoostItemData>(pDataStore);
  success &= LoadData<objects::MiDevilBoostLotData>(pDataStore);
  success &= LoadData<objects::MiDevilEquipmentData>(pDataStore);
  success &= LoadData<objects::MiDevilEquipmentItemData>(pDataStore);
  success &= LoadData<objects::MiDevilFusionData>(pDataStore);
  success &= LoadData<objects::MiDevilLVUpRateData>(pDataStore);
  success &= LoadData<objects::MiDisassemblyData>(pDataStore);
  success &= LoadData<objects::MiDisassemblyTriggerData>(pDataStore);
  success &= LoadData<objects::MiDynamicMapData>(pDataStore);
  success &= LoadData<objects::MiEnchantData>(pDataStore);
  success &= LoadData<objects::MiEquipmentSetData>(pDataStore);
  success &= LoadData<objects::MiExchangeData>(pDataStore);
  success &= LoadData<objects::MiExpertData>(pDataStore);
  success &= LoadData<objects::MiGuardianAssistData>(pDataStore);
  success &= LoadData<objects::MiGuardianLevelData>(pDataStore);
  success &= LoadData<objects::MiGuardianSpecialData>(pDataStore);
  success &= LoadData<objects::MiGuardianUnlockData>(pDataStore);
  success &= LoadData<objects::MiHNPCData>(pDataStore);
  success &= LoadData<objects::MiItemData>(pDataStore);
  success &= LoadData<objects::MiMissionData>(pDataStore);
  success &= LoadData<objects::MiMitamaReunionBonusData>(pDataStore);
  success &= LoadData<objects::MiMitamaReunionSetBonusData>(pDataStore);
  success &= LoadData<objects::MiMitamaUnionBonusData>(pDataStore);
  success &= LoadData<objects::MiModificationData>(pDataStore);
  success &= LoadData<objects::MiModificationExtEffectData>(pDataStore);
  success &= LoadData<objects::MiModificationExtRecipeData>(pDataStore);
  success &= LoadData<objects::MiModificationTriggerData>(pDataStore);
  success &= LoadData<objects::MiModifiedEffectData>(pDataStore);
  success &= LoadData<objects::MiNPCBarterData>(pDataStore);
  success &= LoadData<objects::MiNPCBarterConditionData>(pDataStore);
  success &= LoadData<objects::MiNPCBarterGroupData>(pDataStore);
  success &= LoadData<objects::MiONPCData>(pDataStore);
  success &= LoadData<objects::MiQuestBonusCodeData>(pDataStore);
  success &= LoadData<objects::MiQuestData>(pDataStore);
  success &= LoadData<objects::MiShopProductData>(pDataStore);
  success &= LoadData<objects::MiSItemData>(pDataStore);
  success &= LoadData<objects::MiSkillData>(pDataStore);
  success &= LoadData<objects::MiStatusData>(pDataStore);
  success &= LoadData<objects::MiSynthesisData>(pDataStore);
  success &= LoadData<objects::MiTankData>(pDataStore);
  success &= LoadData<objects::MiTimeLimitData>(pDataStore);
  success &= LoadData<objects::MiTitleData>(pDataStore);
  success &= LoadData<objects::MiTriUnionSpecialData>(pDataStore);
  success &= LoadData<objects::MiUraFieldTowerData>(pDataStore);
  success &= LoadData<objects::MiWarpPointData>(pDataStore);
  success &= LoadData<objects::MiZoneData>(pDataStore);

  if (success) {
    LogDefinitionManagerInfoMsg("Definition loading complete.\n");
  } else {
    LogDefinitionManagerCriticalMsg("Definition loading failed.\n");
  }

  return success;
}

namespace libcomp {
template <>
bool DefinitionManager::RegisterServerSideDefinition<objects::EnchantSetData>(
    const std::shared_ptr<objects::EnchantSetData> &record) {
  uint32_t id = record->GetID();
  if (mEnchantSetData.find(id) != mEnchantSetData.end()) {
    LogDefinitionManagerError([&]() {
      return libcomp::String("Duplicate enchant set encountered: %1\n").Arg(id);
    });
    return false;
  }

  mEnchantSetData[id] = record;

  for (int16_t effectID : record->GetEffects()) {
    if (effectID != 0) {
      mEnchantSetLookup[effectID].push_back(id);
    }
  }

  return true;
}

template <>
bool DefinitionManager::RegisterServerSideDefinition<
    objects::EnchantSpecialData>(
    const std::shared_ptr<objects::EnchantSpecialData> &record) {
  uint32_t id = record->GetID();
  if (mEnchantSpecialData.find(id) != mEnchantSpecialData.end()) {
    LogDefinitionManagerError([&]() {
      return libcomp::String("Duplicate enchant special encountered: %1\n")
          .Arg(id);
    });
    return false;
  }

  mEnchantSpecialData[id] = record;
  mEnchantSpecialLookup[record->GetInputItem()].push_back(id);

  return true;
}

template <>
bool DefinitionManager::RegisterServerSideDefinition<objects::MiSItemData>(
    const std::shared_ptr<objects::MiSItemData> &record) {
  // Register additional tokusei
  uint32_t itemID = record->GetID();
  for (auto tokuseiID : record->GetTokusei()) {
    if (tokuseiID != 0) {
      mSItemTokusei[itemID].insert(tokuseiID);
    }
  }

  return true;
}

template <>
bool DefinitionManager::RegisterServerSideDefinition<objects::MiSStatusData>(
    const std::shared_ptr<objects::MiSStatusData> &record) {
  uint32_t id = record->GetID();
  if (mSStatusData.find(id) != mSStatusData.end()) {
    LogDefinitionManagerError([&]() {
      return libcomp::String("Duplicate s-status encountered: %1\n").Arg(id);
    });
    return false;
  }

  mSStatusData[id] = record;

  return true;
}

template <>
bool DefinitionManager::RegisterServerSideDefinition<objects::Tokusei>(
    const std::shared_ptr<objects::Tokusei> &record) {
  int32_t id = record->GetID();
  if (mTokuseiData.find(id) != mTokuseiData.end()) {
    LogDefinitionManagerError([&]() {
      return libcomp::String("Duplicate tokusei encountered: %1\n").Arg(id);
    });
    return false;
  }

  mTokuseiData[id] = record;

  return true;
}
}  // namespace libcomp

std::shared_ptr<objects::QmpFile> DefinitionManager::LoadQmpFile(
    const libcomp::String &fileName, DataStore *pDataStore) {
  auto path = libcomp::String("/Map/Zone/Model/") + fileName;

  std::vector<char> data = pDataStore->ReadFile(path);
  if (data.empty()) {
    return nullptr;
  }

  std::stringstream ss(std::string(data.begin(), data.end()));

  uint32_t magic;
  ss.read(reinterpret_cast<char *>(&magic), sizeof(magic));

  if (magic != QMP_FORMAT_MAGIC) {
    return nullptr;
  }

  auto file = std::make_shared<objects::QmpFile>();
  if (!file->Load(ss)) {
    return nullptr;
  }

  return file;
}

bool DefinitionManager::LoadBinaryDataHeader(libcomp::ObjectInStream &ois,
                                             const libcomp::String &binaryFile,
                                             uint16_t tablesExpected,
                                             uint16_t &entryCount,
                                             uint16_t &tableCount) {
  ois.stream.read(reinterpret_cast<char *>(&entryCount), sizeof(entryCount));
  ois.stream.read(reinterpret_cast<char *>(&tableCount), sizeof(tableCount));

  if (!ois.stream.good()) {
    LogDefinitionManagerCritical([&]() {
      return libcomp::String("Failed to load/decrypt '%1'.\n").Arg(binaryFile);
    });
    return false;
  }

  if (tablesExpected > 0 && tablesExpected != tableCount) {
    LogDefinitionManagerCritical([&]() {
      return libcomp::String(
                 "Expected %1 table(s) in file '%2' but encountered %3.\n")
          .Arg(tablesExpected)
          .Arg(binaryFile)
          .Arg(tableCount);
    });
    return false;
  }
  return true;
}

void DefinitionManager::PrintLoadResult(const libcomp::String &binaryFile,
                                        bool success, uint16_t entriesExpected,
                                        size_t loadedEntries) {
  if (success) {
    LogDefinitionManagerInfo([&]() {
      return libcomp::String("Successfully loaded %1/%2 records from %3.\n")
          .Arg(loadedEntries)
          .Arg(entriesExpected)
          .Arg(binaryFile);
    });
  } else {
    LogDefinitionManagerError([&]() {
      return libcomp::String("Failed after loading %1/%2 records from %3.\n")
          .Arg(loadedEntries)
          .Arg(entriesExpected)
          .Arg(binaryFile);
    });
  }
}

#ifndef EXOTIC_PLATFORM
namespace libcomp {
template <>
ScriptEngine &ScriptEngine::Using<DefinitionManager>() {
  if (!BindingExists("DefinitionManager")) {
    Sqrat::Class<DefinitionManager> binding(mVM, "DefinitionManager");
    Bind<DefinitionManager>("DefinitionManager", binding);

    // These are needed for some methods.
    Using<objects::MiDevilData>();

    binding.Func("LoadAllData", &DefinitionManager::LoadAllData)
        .Func<std::shared_ptr<objects::MiDevilData> (DefinitionManager::*)(
            uint32_t)>("GetDevilData", &DefinitionManager::GetDevilData)
        // Can't overload because it has the same number of arguments.
        //.Overload<const std::shared_ptr<objects::MiDevilData>
        //(DefinitionManager::*)(const libcomp::String&)>("GetDevilData",
        //&DefinitionManager::GetDevilData)
        ;  // Last call to binding
  }

  return *this;
}
}  // namespace libcomp
#endif  // !EXOTIC_PLATFORM
