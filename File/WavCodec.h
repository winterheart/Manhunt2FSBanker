/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace UTILS {

typedef struct ADPCMChannelStatus {
  int predictor = 0;
  int16_t step_index = 0;
  int32_t prev_sample = 0; // for encoding
} ADPCMChannelStatus;

class WavCodec {
public:
  static uint32_t encode(std::istream &input, std::ostream &output, uint32_t num_channels, uint32_t input_size);

  static uint32_t decode(std::istream &input, std::ostream &output, uint32_t num_channels, uint32_t input_size);

private:
  static int32_t AdpcmImaQtExpandNibble(const std::shared_ptr<ADPCMChannelStatus> &cs, int32_t nibble);
  static uint8_t AdpcmImaQtCompressSample(const std::shared_ptr<ADPCMChannelStatus> &cs, int16_t sample);

  static int16_t AdpcmClipInt16(int a) {
    if ((a + 0x8000U) & ~0xFFFF) {
      return (a >> 31) ^ 0x7FFF;
    }
    return a;
  }

  static std::vector<int16_t> m_adpcm_step_table;
  static std::vector<int8_t> m_adpcm_index_table;
  static uint32_t m_num_samples;
};

} // namespace UTILS
