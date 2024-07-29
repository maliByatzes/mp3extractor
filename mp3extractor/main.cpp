#include "video_parser.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// TODO: parse the file path from command line instead

int main() {
  Parser mp4parser{};

  const std::string file_name{"video1.mp4"};
  // Testing purposes
  try {
    mp4parser.readFileData(file_name);

    if (mp4parser.verifyFileData()) {
      std::cout << "Data is valid.\n";
    } else {
      std::cout << "Data is invalid.\n";
    }
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    std::exit(1);
  }
}
