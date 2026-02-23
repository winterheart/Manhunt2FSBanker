/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "Wav.h"

#include "binio.h"

namespace MH2FSB {

std::ostream &operator<<(std::ostream &out, WavHeader &c) {
  UTILS::bin_write(out, c.ChunkID, 4);
  UTILS::bin_write(out, c.ChunkSize);
  UTILS::bin_write(out, c.FormatID, 4);
  UTILS::bin_write(out, c.Subchunk1ID, 4);
  UTILS::bin_write(out, c.Subchunk1Size);
  UTILS::bin_write(out, c.AudioFormat);
  UTILS::bin_write(out, c.NumChannels);
  UTILS::bin_write(out, c.SampleRate);
  UTILS::bin_write(out, c.BytesRate);
  UTILS::bin_write(out, c.BlockAlign);
  UTILS::bin_write(out, c.BitsPerSample);
  if (c.AudioFormat == WAVE_FORMAT_IMA_XBOX) {
    UTILS::bin_write(out, c.ExtendedDataSize);
    UTILS::bin_write(out, c.ExtendedData, c.ExtendedDataSize);
  }
  UTILS::bin_write(out, c.Subchunk2ID, 4);
  UTILS::bin_write(out, c.Subchunk2Size);

  return out;
}

std::istream &operator>>(std::istream &in, WavHeader &c) {
  UTILS::bin_read(in, c.ChunkID, 4);
  if (c.ChunkID != "RIFF") {
    throw std::runtime_error("File is not a RIFF container");
  }
  UTILS::bin_read(in, c.ChunkSize);
  UTILS::bin_read(in, c.FormatID, 4);
  if (c.ChunkID != "RIFF") {
    throw std::runtime_error("File is not a WAVe file");
  }
  UTILS::bin_read(in, c.Subchunk1ID, 4);
  if (c.Subchunk1ID != "fmt ") {
    throw std::runtime_error("Cannot find 'fmt ' chunk");
  }
  UTILS::bin_read(in, c.Subchunk1Size);
  UTILS::bin_read(in, c.AudioFormat);
  UTILS::bin_read(in, c.NumChannels);
  UTILS::bin_read(in, c.SampleRate);
  UTILS::bin_read(in, c.BytesRate);
  UTILS::bin_read(in, c.BlockAlign);
  UTILS::bin_read(in, c.BitsPerSample);
  if (c.AudioFormat == WAVE_FORMAT_IMA_XBOX) {
    UTILS::bin_read(in, c.ExtendedDataSize);
    UTILS::bin_read(in, c.ExtendedData, c.ExtendedDataSize);
  }

  UTILS::bin_read(in, c.Subchunk2ID, 4);
  if (c.Subchunk2ID != "data") {
    throw std::runtime_error("Cannot find 'data' chunk");
  }
  UTILS::bin_read(in, c.Subchunk2Size);

  return in;
}

} // MH2FSB
