/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <array>
#include <cstdint>
#include <map>

#include "FileContainer.h"

namespace MH2FSB {

/*
Speech types:
Say_ServantName
Address_Many
Address_Pack
Start_Investigate
Say_AreaName
Start_Chase
Start_Melee_Combat
Start_Search
Call_Off_Alert
Probable_Sighting
Probable_Hearing
Taunt_Combat
Enemy_Runs_In_SafeZone
ShoutForHelp

Mumbling
Chat_Statements
Burning_Pain
Burning_Death
Tripwire_Neck
Cancel_Speech

Specific_1
Specific_2
Specific_4
Specific_5
Specific_6
Specific_7
Specific_8
 */

enum ContextNames {
  UNKNOWN_00,             // Not seen
  NEGATIVE_SEARCH_RESULT, // Negative_Search_Result? 9
  DEFINITE_SIGHTING,      // Definite_Sighting? 12
  UNKNOWN_03,             // Only for DIRECTOR_LEO. Probably killing reaction?
  UNKNOWN_04,             // Only for DIRECTOR_LEO. Probably execution reaction?
  RUN_TO_INVESTIGATE,     // Run_To_Investigate? 13
  WALK_TO_INVESTIGATE,    // Walk_To_Investigate? 14
  STOP_AND_LISTEN,        // Stop_And_Listen? 15
  CURIOSITY_NO_RESULT,    // Curiosity_No_Result? 16
  TAUNT_SEARCH,           // Taunt_Search? 17
  POSITIVE_TAUNT_SEARCH,  // Start_Melee_Combat? Taunt_Combat?
  NEGATIVE_TAUNT_SEARCH,  // Disengage from melee battle?
  UNKNOWN_12,             // Urinating?
  TAUNT_CHASE,            // Taunt_Chase 22
  TAUNT_SHORT,            //
  TAUNT_SAFE_ZONE,        // Taunt_Safe_Zone? 24
  TAUNT_BOUNDARY,         //
  TAUNT_PLAYER_DEAD,      // TAUNT_DEATH? Taunt_Death? 25
  JOIN_ATTACK,            // Join_Attack? 26
  WAIT_ENEMY_ALONE,       // Enemy_Runs_In_SafeZone? 27
  WAIT_ENEMY_MULTIPLE,    //
  SNEAK_INVESTIGATE,      // Mumbling? 29
  WAIT_IN_COVER,          //
  SURPRISE,               // Chat_Statements? 31
  GREETINGS,              //
  PLAYER_,                // On failed to hide?
  CLAIM_TERRITORY,        //
  GENERIC_IND,            //
  WHISTLE,                //
  CHAT_STATEMENT,         //
  CHAT_SEARCH,            // Start_Search?
  CHAT_INVESTIGATE,       // Start_Investigate?
  UNKNOWN_32,             // ShoutForHelp? 40
  SHOUT_FOR_ASSISTANCE,   // ShoutForAssistance 41
  LIGHT_PAIN,             // Light_Pain 42
  MEDIUM_PAIN,            // Medium_Pain 43
  HIGH_PAIN,              // High_Pain 44
  LONG_PAIN,              // Long_Pain 45
  GENERIC_DEATH,          // Generic_Death Burning_Pain? 46
  UNKNOWN_39,             // Scream of pain/agony for PLAYER, rage and triumph for DIRECTOR_LEO Generic_Death? 47
  EXECUTION_DEATH,        // Execution_Death? 48 Burning_Death? 49
  GRUNT,                  // Grunt 51
  NEGATIVE_CHASE_RESULT,  // Negative_Chase_Result
  BEGGING_PLEADING,       //
  DEAD_BODY_SEEN,         //
  UNKNOWN_45,             // Exhausted from running?
  UNKNOWN_46,             // Smoking?
  FAILED_SEARCH,          //
  CRAWLSPACE_,            //
  JUMP_REACTION,          //
  CRAWL_REACTION,         // Crawl_Reaction? 58
  CHAT_QUESTION,          // Negative_Chase_Result_Argue? 59
  CHAT_POSITIVE,          // Positive answer in chat Specific_1 60
  CHAT_NEGATIVE,          // Negative answer in chat Specific_2 61
  FLARE_DEATH,            // Specific_3 / Flare_Death 62
  GASCAN_DEATH,           // Specific_4 63
  UNKNOWN_56,             // Vomiting? Specific_5 64
  UNKNOWN_57,             // Interacting with vending machine Specific_6 65
};

class ContextMapResolve {
public:
  static std::string getContextMapName(uint32_t context) {
    if (m_context_map.contains(context)) {
      return m_context_map[context];
    }
    return "";
  }

private:
  static std::map<uint32_t, std::string> m_context_map;
};

class ContextMapBin : public FileContainer {
public:
  std::string getBankName() { return m_bank_name; }
  std::array<uint32_t, 58> &GetEntries() { return m_speech_entries; }

  friend std::ostream &operator<<(std::ostream &out, const ContextMapBin &c);
  friend std::istream &operator>>(std::istream &in, ContextMapBin &c);

private:
  std::string m_bank_name;
  std::array<uint32_t, 58> m_speech_entries;
};

} // namespace MH2FSB
