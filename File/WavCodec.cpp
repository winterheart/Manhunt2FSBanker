/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "WavCodec.h"

#include "binio.h"

#include <algorithm>
#include <memory>

namespace UTILS {

std::vector<int16_t> WavCodec::m_adpcm_step_table = {
    7,     8,     9,     10,    11,    12,    13,    14,    16,    17,    // 10
    19,    21,    23,    25,    28,    31,    34,    37,    41,    45,    // 20
    50,    55,    60,    66,    73,    80,    88,    97,    107,   118,   // 30
    130,   143,   157,   173,   190,   209,   230,   253,   279,   307,   // 40
    337,   371,   408,   449,   494,   544,   598,   658,   724,   796,   // 50
    876,   963,   1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,  // 60
    2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,  // 70
    5894,  6484,  7132,  7845,  8630,  9493,  10442, 11487, 12635, 13899, // 80
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767         // 89
};

std::vector<int8_t> WavCodec::m_adpcm_index_table = {
    -1, -1, -1, -1, 2, 4, 6, 8, // 8
    -1, -1, -1, -1, 2, 4, 6, 8, // 16
};

int32_t WavCodec::AdpcmImaQtExpandNibble(const std::shared_ptr<ADPCMChannelStatus> &cs, int32_t nibble) {
  int step_index;
  int predictor;
  int diff, step;

  step = m_adpcm_step_table[cs->step_index];
  step_index = cs->step_index + m_adpcm_index_table[nibble];
  step_index = std::clamp(step_index, 0, 88);

  diff = step >> 3;
  if (nibble & 4)
    diff += step;
  if (nibble & 2)
    diff += step >> 1;
  if (nibble & 1)
    diff += step >> 2;

  if (nibble & 8)
    predictor = cs->predictor - diff;
  else
    predictor = cs->predictor + diff;

  cs->predictor = AdpcmClipInt16(predictor);
  cs->step_index = step_index;

  return cs->predictor;
}

uint8_t WavCodec::AdpcmImaQtCompressSample(const std::shared_ptr<ADPCMChannelStatus> &cs, int16_t sample) {
  int delta = sample - cs->prev_sample;
  int diff, step = m_adpcm_step_table[cs->step_index];
  int nibble = 8 * (delta < 0);

  delta = abs(delta);
  diff = delta + (step >> 3);

  if (delta >= step) {
    nibble |= 4;
    delta -= step;
  }
  step >>= 1;
  if (delta >= step) {
    nibble |= 2;
    delta -= step;
  }
  step >>= 1;
  if (delta >= step) {
    nibble |= 1;
    delta -= step;
  }
  diff -= delta;

  if (nibble & 8)
    cs->prev_sample -= diff;
  else
    cs->prev_sample += diff;

  cs->prev_sample = AdpcmClipInt16(cs->prev_sample);
  cs->step_index = std::clamp(cs->step_index + m_adpcm_index_table[nibble], 0, 88);

  return nibble;
}

uint32_t WavCodec::encode(std::istream &input, std::ostream &output) {
  auto write_pos = output.tellp();
  while (m_input_size > 0) {
    for (uint32_t ch = 0; ch < m_num_channels; ch++) {
      int16_t sample;
      UTILS::bin_read<int16_t>(input, sample);
      m_input_size -= 2;
      m_channel_status[ch]->prev_sample = sample;
      UTILS::bin_write<int16_t>(output, m_channel_status[ch]->prev_sample);
      UTILS::bin_write<int8_t>(output, m_channel_status[ch]->step_index);
      UTILS::bin_write<int8_t>(output, 0);
    }

    uint32_t samples_count = m_num_samples - 1;
    /* Original encoder compress 63 samples in frame, so last int32 has only 7 samples */
    while (samples_count > 0) {
      uint32_t samples_min = std::min<uint32_t>(samples_count, 8);
      std::vector<int32_t> encoded_frames(m_num_channels, 0);

      for (uint32_t i = 0; i < samples_min; i++) {
        for (uint32_t ch = 0; ch < m_num_channels; ch++) {
          int16_t sample = 0;
          if (m_input_size > 0) {
            m_input_size -= 2;
            UTILS::bin_read<int16_t>(input, sample);
          }
          uint8_t v = AdpcmImaQtCompressSample(m_channel_status[ch], sample);
          encoded_frames[ch] |= v << (4 * i);
        }
      }

      for (auto &sample : encoded_frames) {
        UTILS::bin_write(output, sample);
      }

      samples_count -= samples_min;
    }
  }
  return output.tellp() - write_pos;
}

uint32_t WavCodec::decode(std::istream &input, std::ostream &output) {
  auto write_pos = output.tellp();

  while (m_input_size > 0) {
    for (uint32_t ch = 0; ch < m_num_channels; ch++) {
      int16_t sample;
      uint8_t step_index;
      UTILS::bin_read<int16_t>(input, sample);
      UTILS::bin_read<uint8_t>(input, step_index);
      if (step_index > 88) {
        throw std::runtime_error("Invalid step index");
      }
      input.seekg(1, std::ios_base::cur);
      m_input_size -= 4;
      m_channel_status[ch]->predictor = sample;
      m_channel_status[ch]->step_index = step_index;
      UTILS::bin_write<int16_t>(output, sample);
    }

    uint32_t samples_count = m_num_samples - 1;
    /* Original encoder compress 63 samples in frame, so last int32 has only 7 samples */
    while (samples_count > 0) {
      uint32_t samples_min = std::min(samples_count, 8u);
      std::vector<int32_t> encoded_frames(m_num_channels, 0);

      for (uint32_t ch = 0; ch < m_num_channels; ch++) {
        UTILS::bin_read(input, encoded_frames[ch]);
        m_input_size -= 4;
      }

      for (int i = 0; i < samples_min; i++) {
        for (uint32_t ch = 0; ch < m_num_channels; ch++) {
          int16_t sample = AdpcmImaQtExpandNibble(m_channel_status[ch], encoded_frames[ch] & 0x0F);
          encoded_frames[ch] >>= 4;
          UTILS::bin_write<int16_t>(output, sample);
        }
      }
      samples_count -= samples_min;
    }
  }

  return output.tellp() - write_pos;
}

} // namespace UTILS
