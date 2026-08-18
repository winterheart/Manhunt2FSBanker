/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "Wav.h"

#include "binio.h"

namespace MH2FSB {

std::ostream &operator<<(std::ostream &out, WavHeader &c) {
  uint32_t riff_size = c.m_raw_data_size + 36;
  if (c.m_audio_format == WAVE_FORMAT_IMA_XBOX) {
    riff_size += 4;
  }
  UTILS::bin_write(out, "RIFF", 4);
  UTILS::bin_write(out, riff_size);
  UTILS::bin_write(out, "WAVE", 4);
  UTILS::bin_write(out, "fmt ", 4);
  UTILS::bin_write(out, c.m_fmt_chunk_size);
  UTILS::bin_write(out, c.m_audio_format);
  UTILS::bin_write(out, c.m_num_channels);
  UTILS::bin_write(out, c.m_sample_rate);
  UTILS::bin_write(out, c.m_bytes_rate);
  UTILS::bin_write(out, c.m_block_align);
  UTILS::bin_write(out, c.m_bits_per_sample);
  if (c.m_audio_format == WAVE_FORMAT_IMA_XBOX) {
    UTILS::bin_write<uint16_t>(out, 2);
    UTILS::bin_write<uint16_t>(out, c.m_frame_size);
  }
  UTILS::bin_write(out, "data", 4);
  UTILS::bin_write(out, c.m_raw_data_size);

  return out;
}

std::istream &operator>>(std::istream &in, WavHeader &c) {
  std::string magic;
  uint32_t riff_size;
  uint32_t chunk_id;
  UTILS::bin_read(in, magic, 4);
  if (magic != "RIFF") {
    throw std::runtime_error("File is not a RIFF container");
  }
  UTILS::bin_read(in, riff_size);
  UTILS::bin_read(in, magic, 4);
  riff_size -= 4;
  if (magic != "WAVE") {
    throw std::runtime_error("File is not a WAV file");
  }
  uint32_t chunk_size;
  uint32_t data_position = 0;
  uint32_t failsafe = 1000;
  while (riff_size > 0 && failsafe > 0) {
    UTILS::bin_read(in, chunk_id);
    riff_size -= 4;
    switch (chunk_id) {
    case fmt_: {
      UTILS::bin_read(in, c.m_fmt_chunk_size);
      riff_size -= 4;
      chunk_size = c.m_fmt_chunk_size;

      UTILS::bin_read(in, c.m_audio_format);
      switch (c.m_audio_format) {
        case WAVE_FORMAT_PCM:
        case WAVE_FORMAT_IMA_XBOX:
          break;
        default:
          throw std::runtime_error("Unknown WAVE format");
      }
      UTILS::bin_read(in, c.m_num_channels);
      UTILS::bin_read(in, c.m_sample_rate);
      UTILS::bin_read(in, c.m_bytes_rate);
      UTILS::bin_read(in, c.m_block_align);
      UTILS::bin_read(in, c.m_bits_per_sample);
      riff_size -= 16;
      chunk_size -= 16;
      if (chunk_size > 0) {
        uint16_t extended_size;
        UTILS::bin_read(in, extended_size);
        riff_size -= 2;
        chunk_size -= 2;
        if (extended_size > 0) {
          if (c.m_audio_format == WAVE_FORMAT_IMA_XBOX) {
            UTILS::bin_read(in, c.m_frame_size);
            riff_size -= 2;
            chunk_size -= 2;
          } else {
            // Just skip extended data
            in.seekg(extended_size, std::ios::cur);
            riff_size -= extended_size;
            chunk_size -= extended_size;
          }
        }
      }
      break;
    }
    case data: {
      UTILS::bin_read(in, c.m_raw_data_size);
      riff_size -= 4 + c.m_raw_data_size;
      data_position = in.tellg();
      in.seekg(c.m_raw_data_size, std::ios::cur);
      break;
    }
    default: {
      // Unknown chunk, don't care
      UTILS::bin_read(in, chunk_size);
      in.seekg(chunk_size, std::ios::cur);
      riff_size -= 4 + chunk_size;
      break;
    }
    }
    failsafe--;
  }
  if (failsafe == 0) {
    throw std::runtime_error("Failed to parse WAV header (infinite parse loop occurs)!");
  }
  // Seek to sample data position for convenient
  in.seekg(data_position, std::ios::beg);

  return in;
}

} // MH2FSB
