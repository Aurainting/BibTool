/*!
 * @author Aurumting
 * @date 2025/3/15
 */

#pragma once

#include <vector>

#include "bib_entry.hpp"

namespace bibtool {

inline std::string TypeMode(std::string::const_iterator& it) {
  std::string type_name{};
  while (std::isalnum(*it)) {
    type_name += *it;
    ++it;
  }
  return type_name;
}

inline void AbbrMode(std::string::const_iterator& it,
                     std::unordered_map<std::string, std::string>& bib_abbr) {
  while (*it++ != '{') {
  }
  std::string key{};
  while (std::isalnum(*it)) {
    key += *it;
    ++it;
  }
  while (*it != '"' && *it != '{') {
    ++it;
  }
  ++it;
  std::string value{};
  while (*it != '"' && *it != '}') {
    value += *it;
    ++it;
  }
  bib_abbr[key] = value;
}

inline void EntryMode(std::string::const_iterator& it,
                      BibEntry& entry) {
  std::vector<char> stack{};
  while (*it++ != '{') {
  }
  stack.push_back('{');

  entry.Name().clear();
  while (*it != ',') {
    entry.Name() += *it;
    ++it;
  }

  std::string key{};
  std::string word{};
  for (++it; !stack.empty(); ++it) {
    if (*it == '\\') { // escape character
      word += *it;
      ++it;
      word += *it;
    } else if (std::isalnum(*it)) {
      word += *it;
    } else if (*it == '{' || (*it == '"' && stack.back() != '"')) {
      if (stack.size() > 1) {
        word += *it;
      }
      stack.push_back(*it);
    } else if (*it == '}' || (*it == '"' && stack.back() == '"')) {
      if (stack.size() > 2) {
        word += *it;
      }
      if (*it == '}' && stack.back() != '{') {
        throw std::runtime_error("Parse error: mismatched {}");
      }
      stack.pop_back();
    } else if (stack.size() == 1 && *it == ',') {
      entry.AddFields(key, word);
      key.clear();
      word.clear();
    } else if (stack.size() == 1 && *it == '=') {
      key = word;
      word.clear();
    } else {
      if (stack.size() > 1) {
        word += *it;
      }
    }
  }
  if (!key.empty() && !word.empty()) {
    entry.AddFields(key, word);
  }
}

} // namespace bibtool
