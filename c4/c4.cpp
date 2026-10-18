// c4: Connect 4 CLI tool
// Usage:
//     ./build/c4 bookgen --rules VHD2 --depth 8 --out books/6x5
//     ./build/c4 --config example.toml
#include <iostream>
#include <string>
#include <vector>

#include "CLI11.hpp"
#include "Generator.hpp"
#include "cli.hpp"

static int not_implemented(const CLI::App* sub) {
  std::cerr << "[" << sub->get_name() << "]\n" << sub->config_to_str(true, false);
  std::cerr << sub->get_name() << ": not implemented yet" << std::endl;
  return 2;
}

int main(int argc, char** argv) {
  CLI::App app{"c4: Connect 4 CLI tool"};
  app.require_subcommand(1);
  app.set_config("--config", "", "read options from a .toml file");

  // bookgen
  auto* bookgen = app.add_subcommand("bookgen", "generate the .book");
  bookgen->configurable(); // a [bookgen] section in --config selects this subcommand
  std::string rules, out;
  int depth = 0;
  bookgen->add_option("--rules", rules, "rules the book is for: V, H, D1, D2; VHD1D2 (real game")
      ->required()->check(c4::RULES_VALIDATOR);
  bookgen->add_option("--depth", depth, "depth of the book")
      ->required()->check(CLI::Range(0, 30));
  bookgen->add_option("--out", out, "name of the output file will be <out>-rules-depth.book)")->required();

  CLI11_PARSE(app, argc, argv);
  if (*bookgen) {
    std::vector<GameSolver::Connect4::Position> positions = GameSolver::Connect4::enumerate_positions(depth, c4::parse_rules(rules));
    std::cerr << "enumerated " << positions.size() << " positions" << std::endl;
    return not_implemented(bookgen);  // TODO: solve and pack
  }
  return 0;
}
