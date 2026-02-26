/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <format>
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "manhunt2fsbanker_version.h"
#include "CLI11.hpp"
#include "Dir.h"
#include "Fsb.h"
#include "Wav.h"

void extract(const std::filesystem::path &input_path, const std::filesystem::path &output_path, const bool &recode_wav) {
  const std::filesystem::path& fsb_file = input_path;
  const std::filesystem::path& dir_path = output_path / fsb_file.stem();

  std::filesystem::path dir_file = fsb_file;
  dir_file.replace_extension(".dir");

  std::fstream fsb_stream(fsb_file, std::fstream::in | std::fstream::binary);
  if (!fsb_stream.is_open()) {
    exit(-1);
  }
  std::fstream dir_stream(dir_file, std::fstream::in | std::fstream::binary);
  if (!dir_stream.is_open()) {
    exit(-1);
  }

  MH2FSB::Fsb fsb;
  fsb_stream >> fsb;

  MH2FSB::Dir dir;
  dir_stream >> dir;

  if (dir_file.stem() == "Executions") {
    fsb.ResolveExecutionsNames(dir);
  }
  if (dir_file.stem() == "Scripted") {
    fsb.ResolveScriptedNames(dir);
  }

  dir_stream.close();

  for (const auto& sample : fsb.GetSamples()) {
    std::filesystem::path wav_file = dir_path / sample.GetRealName();
    std::error_code ec;
    std::filesystem::create_directories(wav_file.parent_path(), ec);
    if (ec) {
      std::cerr << ec.message() << std::endl;
      exit(-1);
    }

    std::fstream wav_stream(wav_file, std::fstream::out | std::fstream::binary);
    auto mode = sample.GetMode();
    auto sample_size = sample.GetSize();
    auto format = (mode | MH2FSB::FSOUND_IMAADPCM) ? MH2FSB::WAVE_FORMAT_IMA_XBOX : MH2FSB::WAVE_FORMAT_PCM;
    if (recode_wav && format == MH2FSB::WAVE_FORMAT_IMA_XBOX) {
      format = MH2FSB::WAVE_FORMAT_PCM;
    }
    MH2FSB::WavHeader wav_header(format, sample.GetChannels(), sample.GetFrequency(), sample_size);

    wav_stream << wav_header;

    fsb_stream.seekg(sample.GetOffset(), std::ios::beg);

    std::vector<char> buffer(sample_size);
    fsb_stream.read(buffer.data(), sample_size);
    wav_stream.write(buffer.data(), sample_size);
    wav_stream.close();
  }
  fsb_stream.close();

  YAML::Node node;
  node["fsb"] = fsb;

  std::fstream yaml_stream(dir_path / "fileinfo.yaml", std::fstream::out);
  yaml_stream << node << std::endl;
  yaml_stream.close();
}

void pack(const std::filesystem::path &input_path, const std::filesystem::path &output_path) {
  std::filesystem::path fsb_file = output_path / input_path.stem();
  fsb_file.replace_extension(".fsb");

  MH2FSB::Fsb fsb(input_path);

  std::fstream fsb_stream(fsb_file, std::fstream::out | std::fstream::binary);
  fsb_stream << fsb;

  for (auto& sample : fsb.GetSamples()) {
    MH2FSB::WavHeader wav_header;
    std::fstream wav_stream(input_path / sample.GetRealName(), std::fstream::in | std::fstream::binary);
    wav_stream >> wav_header;
    std::vector<char> buffer;
    auto sample_size = wav_header.GetSubchunk2Size();
    buffer.reserve(sample_size);
    wav_stream.read(buffer.data(), sample_size);
    fsb_stream.write(buffer.data(), sample_size);
    wav_stream.close();
  }
  fsb_stream.close();
}

int main(int argc, char *argv[]) {
  std::filesystem::path input_path;
  std::filesystem::path output_path;
  bool recode_wav = false;

  CLI::App app{"Manhunt2FSBanker - extract/pack FSB files from Rockstar's Manhunt 2 PC game"};
  app.set_version_flag("-v", MANHUNT2FSBANKER_VERSION);
  argv = app.ensure_utf8(argv);
  std::cout << std::format("Manhunt2FSBanker {} https://github.com/winterheart/Manhunt2FSBanker\n"
                           "(c) 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com>\n",
                           app.version())
            << std::endl;

  auto extract_cmd =
      app.add_subcommand("extract", "Extract FSB file")->callback(
        [&]() {
          extract(input_path, output_path, recode_wav);
        });
  extract_cmd->add_option("input", input_path, "Input FSB file(s)")->required()->check(CLI::ExistingFile);
  extract_cmd->add_option("output", output_path, "Output directory");
  extract_cmd->add_flag("-r", recode_wav, "Recode samples to WAV PCM format")->default_val(recode_wav);

  auto pack_cmd =
      app.add_subcommand("pack", "Pack files to FSB file")->callback(
        [&]() {
          pack(input_path, output_path);
      });
  pack_cmd->add_option("input", input_path, "Input directory")->required()->check(CLI::ExistingDirectory);
  pack_cmd->add_option("output", output_path, "Output FSB file");

  CLI11_PARSE(app, argc, argv);

  return 0;
}
