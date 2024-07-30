#include "video_parser.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
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

file_iterator Parser::findAtom(const std::string &atom_name) {
  constexpr int DATA_LEN{4};
  if (atom_name.length() != DATA_LEN) {
    return m_file_data.end();
  }

  auto it = m_file_data.begin();
  std::string buffer{};
  while (it < m_file_data.end() - DATA_LEN) {
    buffer.push_back(static_cast<unsigned char>(*it));

    if (buffer.size() == 4 && buffer == atom_name) {
      return it - DATA_LEN + 1;
    } else if (buffer.size() == 4 && buffer != atom_name) {
      buffer.clear();
    }

    it++;
  }

  return m_file_data.end();
}

bool Parser::verifyFileData() {
  const std::string FILETYPE{"ftyp"};
  auto atom_it = findAtom(FILETYPE);

  if (atom_it != m_file_data.end()) {
    /*
    std::cout << "size: " << extractInteger(atom_it - 4) << '\n';
    std::cout << "type: " << exctractString(atom_it, 4) << '\n';
    std::cout << "major brand: " << exctractString(atom_it + 4, 4) << '\n';
    std::cout << "minor version: " << extractInteger(atom_it + 8) << '\n';
    std::cout << "compatible brand 1: " << exctractString(atom_it + 12, 4) <<
    '\n'; std::cout << "compatible brand 2: " << exctractString(atom_it + 16, 4)
    << '\n'; std::cout << "compatible brand 3: " << exctractString(atom_it + 20,
    4) << '\n'; std::cout << "compatible brand 4: " << exctractString(atom_it +
    24, 4) << '\n';
    */

    uint32_t file_size{extractInteger(atom_it - 4)};
    if (m_file_data.size() < file_size) {
      return false;
    }

    std::string type{exctractString(atom_it, 4)};
    if (type != FILETYPE) {
      return false;
    }

    std::string major_brand{exctractString(atom_it + 4, 4)};
    std::vector<std::string> valid_brands{"isom", "iso2", "mp41", "mp42"};

    if (std::find(valid_brands.begin(), valid_brands.end(), major_brand) ==
        valid_brands.end()) {
      return false;
    }
    return true;
  }

  return false;
}

uint32_t Parser::extractInteger(file_iterator start_it) {
  assert((start_it + 4) != m_file_data.end());

  // big-endian exctraction
  return (*(start_it) << 24) | (*(start_it + 1) << 16) |
         (*(start_it + 2) << 8) | *(start_it + 3);
}

std::string Parser::exctractString(file_iterator start_it, size_t length) {
  assert((start_it + length) != m_file_data.end());

  auto end_it = start_it + length;
  std::string buffer{};
  for (; start_it < end_it; start_it++) {
    buffer.push_back(*start_it);
  }
  return buffer;
}
