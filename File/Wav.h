/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include "FileContainer.h"

namespace MH2FSB {

enum WavFormats {
  WAVE_FORMAT_PCM = 0x0001,
  WAVE_FORMAT_IMA_XBOX = 0x0069,
};

// List only subchunks that we care of
enum WavWAVESubchunks {
  data = 1635017060, // "data" subchunk
  fmt_ = 544501094,  // "fmt " subchunk
};

class WavHeader : public FileContainer {
  uint16_t m_audio_format = WAVE_FORMAT_PCM;
  uint16_t m_num_channels = 1;
  uint32_t m_sample_rate = 44100;

  // Derived fmt values
  uint32_t m_fmt_chunk_size = 16;
  uint16_t m_block_align = 2 * m_num_channels;
  uint32_t m_bytes_rate = m_block_align * m_sample_rate;
  uint16_t m_bits_per_sample = 16;

  // Used for ADPCM IMA XBOX
  uint16_t m_frame_size = 64;

  uint32_t m_raw_data_size = 0;

  std::vector<char> m_raw_data;

  void UpdateState() {
    switch (m_audio_format) {
    case WAVE_FORMAT_IMA_XBOX: {
      m_fmt_chunk_size = 20;
      m_block_align = 36 * m_num_channels;
      m_bytes_rate = m_block_align * m_sample_rate / m_frame_size;
      m_bits_per_sample = 4;
      break;
    }
    default: {
      m_fmt_chunk_size = 16;
      m_block_align = 2 * m_num_channels;
      m_bytes_rate = m_block_align * m_sample_rate;
      m_bits_per_sample = 16;
      break;
    }
    }
  }

public:
  // Assuming that will be WAVE_FORMAT_PCM
  WavHeader() = default;
  WavHeader(uint16_t audio_format, uint16_t num_channels, uint32_t sample_rate, uint32_t raw_data_size)
      : m_audio_format(audio_format), m_num_channels(num_channels), m_sample_rate(sample_rate),
        m_raw_data_size(raw_data_size) {
    UpdateState();
  }

  [[nodiscard]] uint16_t GetNumChannels() const { return m_num_channels; }

  [[nodiscard]] uint16_t GetAudioFormat() const { return m_audio_format; }

  [[nodiscard]] uint32_t GetRawDataSize() const { return m_raw_data_size; }

  void SetRawDataSize(uint32_t raw_data_size) { m_raw_data_size = raw_data_size; }

  void SetAudioFormat(WavFormats audio_format) {
    if (audio_format == WAVE_FORMAT_PCM && m_audio_format == WAVE_FORMAT_IMA_XBOX) {
      m_raw_data_size = sizeof(int16_t) * m_frame_size * m_raw_data_size / 36;
    }
    if (audio_format == WAVE_FORMAT_IMA_XBOX && m_audio_format == WAVE_FORMAT_PCM) {
      m_raw_data_size =
          static_cast<uint32_t>(std::ceil(static_cast<float>(m_raw_data_size) /
                                          static_cast<float>(m_num_channels * (sizeof(int16_t) * m_frame_size)))) *
          36 * m_num_channels;
    }
    m_audio_format = audio_format;
    UpdateState();
  }

  friend std::ostream &operator<<(std::ostream &out, WavHeader &c);
  friend std::istream &operator>>(std::istream &in, WavHeader &c);
};

} // namespace MH2FSB
