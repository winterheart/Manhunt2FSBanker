/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "utils.h"
#include "SpeechLst.h"

TEST(Manhunt2FSBanker, SpeechLst) {
  auto speech_lst_tmp_file = std::filesystem::temp_directory_path() / "speech.lst";

  std::fstream speech_lst_stream("levels/A01_Escape_Asylum/speech.lst", std::ios::binary | std::ios::in);
  std::fstream speech_lst_stream_out(speech_lst_tmp_file, std::ios::binary | std::ios::out);

  ASSERT_TRUE(speech_lst_stream.is_open());
  MH2FSB::SpeechLst speech_lst;
  speech_lst_stream >> speech_lst;
  speech_lst_stream_out << speech_lst;
  speech_lst_stream_out.close();
  EXPECT_TRUE(compare_files("levels/A01_Escape_Asylum/speech.lst", speech_lst_tmp_file));
  std::filesystem::remove(speech_lst_tmp_file);
}
