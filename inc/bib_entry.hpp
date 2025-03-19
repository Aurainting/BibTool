/*!
 * @author Aurumting
 * @date 2025/3/10
 */

#pragma once

#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace bibtool {

class BibEntry {
public:
  explicit BibEntry(std::string entry_type)
      : entry_type_(std::move(entry_type)) {}

  const std::string& Name() const { return name_; }

  std::string& Name() { return name_; }

  void AddFields(const std::string& key, const std::string& value) {
    fields_[key] = value;
  }

  const std::string& Title() const {
    if (fields_.contains("title")) {
      return fields_.at("title");
    }
    return fields_.at("TITLE");
  }

  const std::string& VenueName() const {
    if (fields_.contains("journal")) {
      return fields_.at("journal");
    }
    if (fields_.contains("JOURNAL")) {
      return fields_.at("JOURNAL");
    }
    if (fields_.contains("booktitle")) {
      return fields_.at("booktitle");
    }
    if (fields_.contains("BOOKTITLE")) {
      return fields_.at("BOOKTITLE");
    }
    if (fields_.contains("howpublished")) {
      return fields_.at("howpublished");
    }
    if (fields_.contains("HOWPUBLISHED")) {
      return fields_.at("HOWPUBLISHED");
    }
    return "";
  }

  std::unordered_set<std::string> FieldKey() const {
    std::unordered_set<std::string> res;
    for (const auto& k : fields_ | std::views::keys) {
      res.insert(k);
    }
    return res;
  }

private:
  std::string entry_type_;
  std::string name_;
  std::unordered_map<std::string, std::string> fields_;
};

} // namespace bibtool
