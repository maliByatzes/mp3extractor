#ifndef MP3EXTRACTOR_VIDEO_PARSER_H_
#define MP3EXTRACTOR_VIDEO_PARSER_H_

#include <cstdint>
#include <string>
#include <vector>

class Parser {
public:
  Parser() = default;

  void readFileData(const std::string &file_name);
  std::vector<uint8_t> extractMovieBox();
  std::vector<uint8_t> extractMediaDataBox();

private:
  std::vector<uint8_t> m_file_data{};
};

#endif
