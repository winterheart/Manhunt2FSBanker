/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "SpeechLst.h"

bool compare_files(const std::filesystem::path &a, const std::filesystem::path &b) {
  std::ifstream fa(a, std::ios::binary | std::ios::ate);
  std::ifstream fb(b, std::ios::binary | std::ios::ate);

  if (fa.fail() || fb.fail()) {
    return false;
  }

  if (fa.tellg() != fb.tellg()) {
    return false;
  }

  fa.seekg(0, std::ios::beg);
  fb.seekg(0, std::ios::beg);

  return std::equal(std::istreambuf_iterator<char>(fa.rdbuf()), std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(fb.rdbuf()));
}

TEST(Manhunt2FSBanker, SpeechLst) {
  auto speech_lst_tmp_file = std::filesystem::temp_directory_path() / "speech.lst";

  std::fstream speech_lst_stream("Samples/speech.lst", std::ios::binary | std::ios::in);
  std::fstream speech_lst_stream_out(speech_lst_tmp_file, std::ios::binary | std::ios::out);

  ASSERT_TRUE(speech_lst_stream.is_open());
  MH2FSB::SpeechLst speech_lst;
  speech_lst_stream >> speech_lst;
  speech_lst_stream_out << speech_lst;
  speech_lst_stream_out.close();
  EXPECT_TRUE(compare_files("Samples/speech.lst", speech_lst_tmp_file));
  std::filesystem::remove(speech_lst_tmp_file);
}
