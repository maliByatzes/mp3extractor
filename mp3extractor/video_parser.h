#ifndef MP3EXTRACTOR_VIDEO_PARSER_H_
#define MP3EXTRACTOR_VIDEO_PARSER_H_

#include <cstdint>
#include <string>
#include <vector>

using file_iterator = std::vector<uint8_t>::const_iterator;

class Parser {
public:
  Parser() = default;

  void readFileData(const std::string &file_name);
  bool verifyFileData();
  // std::vector<uint8_t> extractMediaDataBox();

private:
  std::vector<uint8_t> m_file_data{};

  file_iterator findAtom(const std::string &atom_name);
  uint32_t extractInteger(file_iterator start_it);
};

#endif
