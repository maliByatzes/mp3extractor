#include "video_parser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

void Parser::readFileData(const std::string &file_name) {
  std::vector<uint8_t> file_data{};
  std::ifstream istrm(file_name, std::ios::binary);

  if (!istrm.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_name);
  }

  istrm.unsetf(std::ios::skipws);

  istrm.seekg(0, std::ios::end);
  std::ifstream::pos_type file_length{ istrm.tellg() };
  istrm.seekg(0, std::ios::beg);

  std::cout << "File length: " << file_length << '\n';
}
