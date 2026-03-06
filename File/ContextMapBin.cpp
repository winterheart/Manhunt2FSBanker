/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "ContextMapBin.h"

#include "binio.h"

namespace MH2FSB {

/**
 * Map for speech types.
 * Format is DD_[(Q|U)_]NAME where
 * DD - number of speech type in contextmap.bin
 * Q - (Questionable) just guessed name
 * U - (Unknown) unknown speech
 * NAME - name discovered from script engine
 */
std::map<uint32_t, std::string> ContextMapResolve::m_context_map = {
    {UNKNOWN_00, "00_Unknown"},
    {NEGATIVE_SEARCH_RESULT, "01_Negative_Search_Result"},
    {DEFINITE_SIGHTING, "02_Definite_Sighting"},
    {UNKNOWN_03, "03_Q_Killing_Reaction"},
    {UNKNOWN_04, "04_Q_Execution_Reaction"},
    {RUN_TO_INVESTIGATE, "05_Run_To_Investigate"},
    {WALK_TO_INVESTIGATE, "06_Walk_To_Investigate"},
    {STOP_AND_LISTEN, "07_Stop_And_Listen"},
    {CURIOSITY_NO_RESULT, "08_Curiosity_No_Result"},
    {TAUNT_SEARCH, "09_Taunt_Search"},
    {POSITIVE_TAUNT_SEARCH, "10_Q_Start_Melee_Combat"},
    {NEGATIVE_TAUNT_SEARCH, "11_Q_Leave_Melee_Combat"},
    {UNKNOWN_12, "12_Urinating"},
    {TAUNT_CHASE, "13_Taunt_Chase"},
    {TAUNT_SHORT, "14_Taunt_Short"},
    {TAUNT_SAFE_ZONE, "15_Taunt_Safe_Zone"},
    {TAUNT_BOUNDARY, "16_Taunt_Boundary"},
    {TAUNT_PLAYER_DEAD, "17_Taunt_Death"},
    {JOIN_ATTACK, "18_Join_Attack"},
    {WAIT_ENEMY_ALONE, "19_Wait_Enemy_Alone"},
    {WAIT_ENEMY_MULTIPLE, "20_Wait_Enemy_Multiple"},
    {SNEAK_INVESTIGATE, "21_Q_Mumbling"},
    {WAIT_IN_COVER, "22_Wait_In_Cover"},
    {SURPRISE, "23_Surprise"},
    {GREETINGS, "24_Greetings"},
    {PLAYER_, "25_Player_"},
    {CLAIM_TERRITORY, "26_Claim_Territory"},
    {GENERIC_IND, "27_Generic_Ind"},
    {WHISTLE, "28_Whistle"},
    {CHAT_STATEMENT, "29_Q_Chat_Statements"}, // Chat_Statements
    {CHAT_SEARCH, "30_Q_Start_Search"},
    {CHAT_INVESTIGATE, "31_Q_Start_Investigate"},
    {UNKNOWN_32, "32_Q_ShoutForHelp"},
    {SHOUT_FOR_ASSISTANCE, "33_ShoutForAssistance"},
    {LIGHT_PAIN, "34_Light_Pain"},
    {MEDIUM_PAIN, "35_Medium_Pain"},
    {HIGH_PAIN, "36_High_Pain"},
    {LONG_PAIN, "37_Long_Pain"},
    {GENERIC_DEATH, "38_Q_Burning_Pain"},
    {UNKNOWN_39, "39_Q_Generic_Death"},
    {EXECUTION_DEATH, "40_Q_Execution_Death"},
    {GRUNT, "41_Grunt"},
    {NEGATIVE_CHASE_RESULT, "42_Negative_Chase_Result"},
    {BEGGING_PLEADING, "43_Begging_Pleading"},
    {DEAD_BODY_SEEN, "44_Dead_Body_Seen"},
    {UNKNOWN_45, "45_Q_Exhausted_From_Running"},
    {UNKNOWN_46, "46_Q_Smoking"},
    {FAILED_SEARCH, "47_Failed_Search"},
    {CRAWLSPACE_, "48_Crawlspace_"},
    {JUMP_REACTION, "49_Jump_Reaction"},
    {CRAWL_REACTION, "50_Crawl_Reaction"},
    {CHAT_QUESTION, "51_Chat_Question"},
    {CHAT_POSITIVE, "52_Chat_Positive"},
    {CHAT_NEGATIVE, "53_Chat_Negative"},
    {FLARE_DEATH, "54_Specific_3"},
    {GASCAN_DEATH, "55_Specific_4"},
    {UNKNOWN_56, "56_Q_Vomiting"},
    {UNKNOWN_57, "57_Q_Using_Vending_Machine"},
};

std::ostream &operator<<(std::ostream &out, const ContextMapBin &c) {
  UTILS::bin_write(out, c.m_bank_name, 32);
  for (auto i : c.m_speech_entries) {
    UTILS::bin_write(out, i);
  }
  return out;
}

std::istream &operator>>(std::istream &in, ContextMapBin &c) {
  UTILS::bin_read(in, c.m_bank_name, 32);
  for (auto &i : c.m_speech_entries) {
    UTILS::bin_read(in, i);
  }
  return in;
}

} // namespace MH2FSB
