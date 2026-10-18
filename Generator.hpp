#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "Position.hpp"

#include <string>
#include <vector>

namespace GameSolver {
namespace Connect4 {

static constexpr int MAX_BOOK_DEPTH = 14;
static constexpr int BOOK_LOG_SIZE = 29;

std::vector<Position> enumerate_positions(int depth, Rules book_rules);

std::vector<int> solve_positions(const std::vector<Position> &positions, Rules book_rules, bool weak);

void save_opening_book(const std::vector<Position> &positions, const std::vector<int> &scores,
                       int depth, Rules book_rules, const std::string &file);

} // namespace Connect4
} // namespace GameSolver
#endif
