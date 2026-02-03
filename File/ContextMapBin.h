/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <array>
#include <cstdint>
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

enum class ContextNames {
  UNKNOWN_00,
  NEGATIVE_SEARCH_RESULT, // Negative_Search_Result?
  DEFINITE_SIGHTING, // Definite_Sighting
  UNKNOWN_03,
  UNKNOWN_04,
  RUN_TO_INVESTIGATE, //
  WALK_TO_INVESTIGATE, // Walk_To_Investigate
  STOP_AND_LISTEN, // Stop_And_Listen
  CURIOSITY_NO_RESULT, // Curiosity_No_Result
  TAUNT_SEARCH, // Taunt_Search
  POSITIVE_TAUNT_SEARCH,
  NEGATIVE_TAUNT_SEARCH,
  UNKNOWN_12,
  TAUNT_CHASE, // Taunt_Chase
  TAUNT_SHORT,
  TAUNT_SAFE_ZONE, //
  TAUNT_BOUNDARY,
  TAUNT_PLAYER_DEAD, // TAUNT_DEATH?
  JOIN_ATTACK, // Join_Attack
  WAIT_ENEMY_ALONE,
  WAIT_ENEMY_MULTIPLE,
  SNEAK_INVESTIGATE,
  WAIT_IN_COVER,
  SURPRISE,
  GREETINGS,
  PLAYER_,
  CLAIM_TERRITORY,
  GENERIC_IND,
  WHISTLE,
  CHAT_STATEMENT, //
  CHAT_SEARCH,
  CHAT_INVESTIGATE,
  UNKNOWN_32,
  SHOUT_FOR_ASSISTANCE, // ShoutForAssistance
  LIGHT_PAIN, // Light_Pain
  MEDIUM_PAIN, // Medium_Pain
  HIGH_PAIN, // High_Pain
  LONG_PAIN, // Long_Pain
  GENERIC_DEATH, // Generic_Death
  UNKNOWN_39,
  EXECUTION_DEATH, // Execution_Death
  GRUNT, // Grunt
  NEGATIVE_CHASE_RESULT, //
  BEGGING_PLEADING,
  DEAD_BODY_SEEN,
  UNKNOWN_45,
  UNKNOWN_46,
  FAILED_SEARCH,
  CRAWLSPACE_,
  JUMP_REACTION,
  CRAWL_REACTION, //
  CHAT_QUESTION,
  CHAT_POSITION,
  CHAT_NEGATIVE,
  FLARE_DEATH, // Flare_Death
  GASCAN_DEATH,
  UNKNOWN_56,
  UNKNOWN_57,
};

class ContextMapBin : public FileContainer {
  public:
    friend std::ostream& operator<<(std::ostream& out, const ContextMapBin& c);
    friend std::istream& operator>>(std::istream& in, ContextMapBin& c);

  private:
    std::string m_bank_name;
    std::array<uint32_t, 58> m_speech_entries;
};

} // MH2FSB
