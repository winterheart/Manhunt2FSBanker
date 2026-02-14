/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "utils.h"
#include "ContextMapBin.h"

TEST(Manhunt2FSBanker, ContextMapBin) {
  auto context_map_bin_tmp_file = std::filesystem::temp_directory_path() / "context_map.bin";

  std::fstream context_map_bin_stream("audio/PC/Speech/ASYLUM_1_1/context_map.bin", std::ios::binary | std::ios::in);
  std::fstream context_map_bin_stream_out(context_map_bin_tmp_file, std::ios::binary | std::ios::out);

  ASSERT_TRUE(context_map_bin_stream.is_open());
  MH2FSB::ContextMapBin context_map_bin;
  context_map_bin_stream >> context_map_bin;
  context_map_bin_stream_out << context_map_bin;
  context_map_bin_stream_out.close();
  EXPECT_TRUE(compare_files("audio/PC/Speech/ASYLUM_1_1/context_map.bin", context_map_bin_tmp_file));
  std::filesystem::remove(context_map_bin_tmp_file);
}
