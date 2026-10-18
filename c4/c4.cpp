// c4: Connect 4 CLI tool
// Usage:
//     ./build/c4 bookgen --rules VHD2 --depth 8 --out books/6x5
//     ./build/c4 --config example.toml
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "CLI11.hpp"
#include "Generator.hpp"
#include "cli.hpp"
#include "position_counts.hpp"
#include "progress_bar.hpp"

int main(int argc, char** argv) {
  CLI::App app{"c4: Connect 4 CLI tool"};
  app.require_subcommand(1);
  app.set_config("--config", "", "read options from a .toml file");

  // bookgen
  auto* bookgen = app.add_subcommand("bookgen", "generate the .book");
  bookgen->configurable(); // a [bookgen] section in --config selects this subcommand
  std::string rules, out;
  int depth = 0;
  bookgen->add_option("--rules", rules, "rules the book is for: V, H, D1, D2; VHD1D2 (real game)")
      ->required()->check(c4::RULES_VALIDATOR);
  bookgen->add_option("--depth", depth, "depth of the book")
      ->required()->check(CLI::Range(0, GameSolver::Connect4::MAX_BOOK_DEPTH));
  bookgen->add_option("--out", out, "output file prefix: writes <out>-<rules>-<depth>.book")->required();

  CLI11_PARSE(app, argc, argv);
  if (*bookgen) {
    GameSolver::Connect4::Rules book_rules = c4::parse_rules(rules);
    c4::progress::start("enumerate", c4::count_positions(depth, book_rules));
    std::vector<GameSolver::Connect4::Position> positions = GameSolver::Connect4::enumerate_positions(depth, book_rules);
    c4::progress::finish();
    positions.erase(positions.begin()); // skip the empty board: is the same as old code does (TODO: maybe reconsider erase this line)
    std::cerr << "enumerated " << positions.size() << " positions" << std::endl;

    c4::progress::start("solve", positions.size());
    std::vector<int> scores = GameSolver::Connect4::solve_positions(positions, book_rules, true);
    c4::progress::finish();
    std::cerr << "solved " << scores.size() << " positions" << std::endl;

    std::filesystem::path file = out + "-" + c4::rules_name(book_rules) + "-" + std::to_string(depth) + ".book";
    if (file.has_parent_path()) std::filesystem::create_directories(file.parent_path());
    GameSolver::Connect4::save_opening_book(positions, scores, depth, book_rules, file.string());
    std::cerr << "saved " << file.string() << std::endl;
  }
  return 0;
}
