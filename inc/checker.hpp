/*!
 * @author Aurumting
 * @date 2025/3/15
 */

#pragma once

#include <cctype>
#include <numeric>
#include <vector>

#include "bib_entry.hpp"

namespace bibtool {

inline double StringSimilarity(const std::string& str1,
                               const std::string& str2) {
  const auto& len1 = str1.size();
  const auto& len2 = str2.size();

  if (len1 == 0 && len2 == 0) {
    return 1.0;
  }
  if (len1 == 0 || len2 == 0) {
    return 0.0;
  }

  std::vector<std::size_t> prev(len2 + 1), curr(len2 + 1);
  std::iota(prev.begin(), prev.end(), 0);

  for (std::size_t i = 1; i <= len1; ++i) {
    curr[0] = i;
    for (std::size_t j = 1; j <= len2; ++j) {
      const auto c1 = static_cast<char>(std::tolower(str1[i - 1]));
      const auto c2 = static_cast<char>(std::tolower(str2[j - 1]));
      const std::size_t cost = c1 == c2 ? 0 : 1;

      curr[j] =
          std::min(std::min(prev[j] + 1, curr[j - 1] + 1), prev[j - 1] + cost);
    }
    std::swap(prev, curr);
  }

  return 1.0 - static_cast<double>(prev[len2]) /
                   static_cast<double>(std::max(len1, len2));
}

inline std::vector<std::pair<std::string, std::string>>
FindDuplicate(const std::vector<BibEntry>& bib_entries,
              const double threshold = 0.85) {
  std::vector<std::pair<std::string, std::string>> duplicates;
  for (std::size_t i = 0; i < bib_entries.size(); ++i) {
    for (std::size_t j = 0; j < bib_entries.size(); ++j) {
      if (i == j) {
        continue;
      }
      if (StringSimilarity(bib_entries[i].Title(), bib_entries[j].Title()) >
          threshold) {
        duplicates.emplace_back(bib_entries[i].Name(), bib_entries[j].Name());
      }
    }
  }
  return duplicates;
}

inline std::vector<std::pair<std::string, std::string>>
FindDuplicate(const std::unordered_map<std::string, std::string>& bib_abbr,
              const std::vector<BibEntry>& bib_entries,
              const double threshold = 0.85) {
  std::vector<std::pair<std::string, std::string>> duplicates;
  for (const auto& entry : bib_entries) {
    for (const auto& [key, value] : bib_abbr) {
      if (StringSimilarity(entry.VenueName(), value) > threshold) {
        duplicates.emplace_back(entry.Name(), key);
      }
    }
  }
  return duplicates;
}

inline std::vector<std::pair<std::string, std::string>>
CheckField(const std::vector<BibEntry>& bib_entries,
           const double threshold = 0.85) {
  std::vector<std::pair<std::string, std::string>> res;
  std::vector done(bib_entries.size(), false);

  for (int i = 0; i < bib_entries.size(); ++i) {
    if (done[i]) {
      continue;
    }

    const auto field_key = bib_entries[i].FieldKey();
    done[i] = true;
    for (int j = 0; j < bib_entries.size(); ++j) {
      if (i == j) {
        continue;
      }

      if (StringSimilarity(bib_entries[i].VenueName(),
                           bib_entries[j].VenueName()) > threshold) {
        if (field_key != bib_entries[j].FieldKey()) {
          res.emplace_back(bib_entries[i].Name(), bib_entries[j].Name());
        }
        done[j] = true;
      }
    }
  }
  return res;
}

} // namespace bibtool
