// Command-line helpers for c4: parsing and validation of option values.
#pragma once
#include <stdexcept>
#include <string>

#include "CLI11.hpp"
#include "Position.hpp"

namespace c4 {

using GameSolver::Connect4::Rules;

inline Rules parse_rules(const std::string& text) {
  static const struct { const char* letters; unsigned bit; } DIRECTIONS[] = {
    {"D1", GameSolver::Connect4::LINE_D1_BIT}, {"D2", GameSolver::Connect4::LINE_D2_BIT},
    {"V", GameSolver::Connect4::LINE_V_BIT},   {"H", GameSolver::Connect4::LINE_H_BIT},
  };
  unsigned mask = 0;
  size_t i = 0;
  while (i < text.size()) {
    bool found = false;
    for (const auto& d : DIRECTIONS) {
      size_t n = std::char_traits<char>::length(d.letters);
      if (text.compare(i, n, d.letters) == 0) {
        if (mask & d.bit) throw std::invalid_argument("repeated direction in rules: " + text);
        mask |= d.bit;
        i += n;
        found = true;
        break;
      }
    }
    if (!found) throw std::invalid_argument("invalid rules: " + text + " (letters: V, H, D1, D2)");
  }
  if (mask == 0) throw std::invalid_argument("rules cannot be empty");
  return Rules{mask};
}

inline const CLI::Validator RULES_VALIDATOR(
    [](const std::string& s) {
      try { parse_rules(s); return std::string(); } catch (const std::exception& e) { return std::string(e.what()); }
    },
    "RULES");

}  // namespace c4
