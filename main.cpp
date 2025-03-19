#include <iostream>

#include "checker.hpp"
#include "file_utils.hpp"
#include "parser.hpp"

int main() {
  // const auto& bib_str = bibtool::ReadFile("../example/mybib.bib");
  const auto& bib_str = bibtool::ReadFile("../example/references.bib");

  std::unordered_map<std::string, std::string> bib_abbr;
  std::vector<bibtool::BibEntry> bib_entries;

  // Parse
  for (auto it = bib_str.cbegin(); it != bib_str.cend(); ++it) {
    if (*it == '@') {
      if (const auto entry_type = bibtool::TypeMode(++it);
          entry_type == "string" || entry_type == "STRING") {
        bibtool::AbbrMode(it, bib_abbr);
      } else {
        bibtool::BibEntry entry(entry_type);
        EntryMode(it, entry);
        bib_entries.push_back(entry);
      }
    }
  }

  // Check
  if (const auto& duplicates = FindDuplicate(bib_entries);
      !duplicates.empty()) {
    for (const auto& [i, j] : duplicates) {
      std::cout << i << " and " << j << " are duplicated!\n";
    }
    std::cout << std::endl;
  }
  if (const auto& duplicates = FindDuplicate(bib_abbr, bib_entries);
      !duplicates.empty()) {
    for (const auto& [i, j] : duplicates) {
      std::cout << i << " and " << j << " are duplicated!\n";
    }
    std::cout << std::endl;
  }
  if (const auto& mis_field = CheckField(bib_entries); !mis_field.empty()) {
    for (const auto& [i, j] : mis_field) {
      std::cout << i << " and " << j << " field keys are not the same!\n";
    }
    std::cout << std::endl;
  }

  return 0;
}
