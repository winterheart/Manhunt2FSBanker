/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <cstdint>
#include <string>

#include "FileContainer.h"

namespace MH2FSB {

enum WavFormats {
  WAVE_FORMAT_PCM = 0x0001,
  WAVE_FORMAT_IMA_XBOX = 0x0069,
};

class WavHeader : public FileContainer {
  std::string ChunkID = "RIFF";     // RIFF Header
  uint32_t ChunkSize = 0;           // RIFF Chunk Size
  std::string FormatID = "WAVE";    // WAVE Header
  std::string Subchunk1ID = "fmt "; // fmt Header
  uint32_t Subchunk1Size = 20;
  uint16_t AudioFormat = 1;
  uint16_t NumChannels = 2;
  uint32_t SampleRate = 44100;
  uint32_t BytesRate = 0; // SampleRate * blockAlign
  uint16_t BlockAlign = 36;
  uint16_t BitsPerSample = 16;
  uint16_t ExtendedDataSize = 0;
  uint16_t ExtendedData = 0;
  std::string Subchunk2ID = "data";
  uint32_t Subchunk2Size = 0; // NumSamples * NumChannels * BitsPerSample/8

public:
  WavHeader() = default;
  WavHeader(uint32_t chunk_size, uint32_t audio_format, uint32_t num_channels, uint32_t sample_rate,
            uint32_t block_align, uint16_t bits_per_sample, uint32_t subchunk2_size)
      : ChunkSize(chunk_size), AudioFormat(audio_format), NumChannels(num_channels), SampleRate(sample_rate),
        BlockAlign(block_align), BitsPerSample(bits_per_sample), Subchunk2Size(subchunk2_size) {
    BytesRate = 49612;
    if (AudioFormat == WAVE_FORMAT_IMA_XBOX) {
      ExtendedDataSize = 2;
      ExtendedData = 64;
    }
  }

  [[nodiscard]] uint32_t GetSubchunk2Size() const { return Subchunk2Size; }

  friend std::ostream &operator<<(std::ostream &out, WavHeader &c);
  friend std::istream &operator>>(std::istream &in, WavHeader &c);
};

} // namespace MH2FSB
