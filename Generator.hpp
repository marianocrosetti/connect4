#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "Position.hpp"

#include <vector>

namespace GameSolver {
namespace Connect4 {

/**
 * All positions with at most depth stones. The first one is the empty board.
 * Legality uses the real rules: a move that completes a line ends the game, so it is not followed.
 * A position and its mirror are listed once if book_rules allow merging mirrors (same book key).
 * Order: depth first, columns left to right.
 */
std::vector<Position> enumerate_positions(int depth, Rules book_rules);

} // namespace Connect4
} // namespace GameSolver
#endif
