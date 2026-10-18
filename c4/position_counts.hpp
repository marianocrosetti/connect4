// Number of positions that enumerate_positions lists on a 6x5 board, by number of stones (index = stones).
// Measured with enumerate_positions itself (2026-10-18). Used as the total of the enumeration progress bar.
#pragma once
#include <cstdint>

#include "Position.hpp"

namespace c4 {

static_assert(GameSolver::Connect4::Position::WIDTH == 6 && GameSolver::Connect4::Position::HEIGHT == 5,
              "position counts are for the 6x5 board");

constexpr int MAX_COUNTED_DEPTH = 16;

constexpr struct {
  int stones;
  int64_t mirrors_merged;
  int64_t mirrors_apart;
} POSITION_COUNTS[MAX_COUNTED_DEPTH + 1] = {
  //   mirrors_merged mirrors_apart         mirrors_merged       mirrors_apart
    { 0,            1,            1},  //   0.00 s      1 MB |   0.00 s      1 MB
    { 1,            3,            6},  //   0.00 s      1 MB |   0.00 s      1 MB
    { 2,           18,           36},  //   0.00 s      1 MB |   0.00 s      1 MB
    { 3,           78,          156},  //   0.00 s      1 MB |   0.00 s      1 MB
    { 4,          330,          651},  //   0.00 s      1 MB |   0.00 s      1 MB
    { 5,        1'128,        2'256},  //   0.00 s      1 MB |   0.00 s      2 MB
    { 6,        3'935,        7'870},  //   0.00 s      2 MB |   0.00 s      2 MB
    { 7,       11'871,       23'742},  //   0.00 s      3 MB |   0.00 s      5 MB
    { 8,       35'757,       71'463},  //   0.00 s      6 MB |   0.01 s     10 MB
    { 9,       93'125,      186'250},  //   0.01 s     15 MB |   0.03 s     32 MB
    {10,      243'663,      487'326},  //   0.04 s     38 MB |   0.12 s     77 MB
    {11,      559'399,    1'118'798},  //   0.21 s     98 MB |   0.56 s    198 MB
    {12,    1'295'022,    2'589'786},  //   0.66 s    265 MB |   1.75 s    531 MB
    {13,    2'630'303,    5'260'606},  //   1.75 s    552 MB |   3.62 s   1106 MB
    {14,    5'414'035,   10'828'070},  //   4.20 s   1135 MB |   7.89 s   2271 MB
    {15,    9'750'176,   19'500'352},  //   8.93 s   2242 MB |  15.19 s   4257 MB
    {16,   17'798'742,   35'596'740},  //  18.45 s   4270 MB |  35.16 s   6492 MB
};

inline int64_t count_positions(int depth, GameSolver::Connect4::Rules rules) {
  if (depth > MAX_COUNTED_DEPTH) return -1;
  int64_t total = 0;
  for (int d = 0; d <= depth; d++)
    total += rules.can_merge_mirrors() ? POSITION_COUNTS[d].mirrors_merged : POSITION_COUNTS[d].mirrors_apart;
  return total;
}

}  // namespace c4
