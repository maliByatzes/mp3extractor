#include "video_parser.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

void Parser::readFileData(const std::string &file_name) {
  std::ifstream istrm(file_name, std::ios::binary);

  if (!istrm.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_name);
  }

  istrm.unsetf(std::ios::skipws);

  istrm.seekg(0, std::ios::end);
  std::ifstream::pos_type file_length{istrm.tellg()};
  istrm.seekg(0, std::ios::beg);

  std::cout << "File length: " << file_length << '\n';

  m_file_data.resize(file_length);

  istrm.read(reinterpret_cast<char *>(m_file_data.data()), file_length);
  if (istrm.gcount() != file_length) {
    throw std::runtime_error("Could not read the entire file: " + file_name);
  }

  istrm.close();
}

std::vector<uint8_t>::const_iterator
Parser::findAtom(const std::string &atom_name) {
  constexpr int data_len{4};
  if (atom_name.length() != data_len) {
    return m_file_data.end();
  }

  auto it = m_file_data.begin();
  std::string buffer{};
  while (it < m_file_data.end() - data_len) {
    buffer.push_back(static_cast<unsigned char>(*it));

    if (buffer.size() == 4 && buffer == atom_name) {
      return it;
    } else if (buffer.size() == 4 && buffer != atom_name) {
      buffer.clear();
    }

    it++;
  }

  return m_file_data.end();
}

bool Parser::verifyFileData() {
  auto atom_it = findAtom("ftyp");

  if (atom_it != m_file_data.end()) {
    std::cout << *atom_it << '\n';
    return true;
  }

  return false;
}
