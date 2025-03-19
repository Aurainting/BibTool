/*!
 * @author Aurumting
 * @date 2025/3/3
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace bibtool {

inline std::string ReadFile(const std::string& file_path) {
  std::ifstream in_file(file_path);
  if (!in_file.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_path);
  }

  std::vector<char> buff(in_file.seekg(0, std::ios::end).tellg());
  in_file.seekg(0).read(buff.data(), static_cast<std::streamsize>(buff.size()));
  in_file.close();

  return {buff.cbegin(), buff.cend()};
}

} // namespace bibtool
