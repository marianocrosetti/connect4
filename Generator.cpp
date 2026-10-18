#include "Generator.hpp"
#include "Progress.hpp"
#include "OpeningBook.hpp"
#include "Solver.hpp"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace GameSolver {
namespace Connect4 {

/**
 * Explore and store all possible position under a given depth.
 * symetric positions are stored only once.
 */
static void explore(const Position &P, const int depth, const Rules book_rules,
                    std::unordered_set<uint64_t> &visited, std::vector<Position> &positions) {
  uint64_t key = P.key3(book_rules);
  if(!visited.insert(key).second)
    return; // already explored position

  int nb_moves = P.nbMoves();
  if(nb_moves <= depth)
  {
    positions.push_back(P);
    report_progress(positions.size());
  }
  if(nb_moves >= depth) return;  // do not explore at further depth

  for(int i = 0; i < Position::WIDTH; i++) // explore all possible moves
    if(P.canPlay(i) && !P.isWinningMove(i, REAL_RULES)) {
      Position P2(P);
      P2.playCol(i);
      explore(P2, depth, book_rules, visited, positions);
    }
}

std::vector<Position> enumerate_positions(int depth, Rules book_rules) {
  std::unordered_set<uint64_t> visited;
  std::vector<Position> positions;
  explore(Position(), depth, book_rules, visited, positions);
  return positions;
}

std::vector<int> solve_positions(const std::vector<Position> &positions, Rules book_rules, bool weak) {
  Solver solver(book_rules);
  std::vector<int> scores;
  scores.reserve(positions.size());
  for(const Position &P : positions) {
    scores.push_back(solver.solve(P, weak));
    report_progress(scores.size());
  }
  return scores;
}

void save_opening_book(const std::vector<Position> &positions, const std::vector<int> &scores,
                       int depth, Rules book_rules, const std::string &file) {
  if(depth > MAX_BOOK_DEPTH) throw std::invalid_argument("book depth " + std::to_string(depth) + " > MAX_BOOK_DEPTH");
  for(int score : scores)
    if(score < Position::MIN_SCORE || score > Position::MAX_SCORE)
      throw std::invalid_argument("score " + std::to_string(score) + " out of [MIN_SCORE, MAX_SCORE]");
  static constexpr double LOG_3 = 1.58496250072; // log2(3)
  using BookTable = TranspositionTable<uint_t<int((MAX_BOOK_DEPTH + Position::WIDTH - 1) * LOG_3) + 1 - BOOK_LOG_SIZE>,
                                       Position::position_t, uint8_t, BOOK_LOG_SIZE>;
  BookTable *table = new BookTable();
  for(size_t i = 0; i < positions.size(); i++)
    table->put(positions[i].key3(book_rules), scores[i] - Position::MIN_SCORE + 1);

  OpeningBook book{Position::WIDTH, Position::HEIGHT, depth, table};
  book.save(file);
}

} // namespace Connect4
} // namespace GameSolver
