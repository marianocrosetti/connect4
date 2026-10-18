#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include <cstdint>
#include <functional>

namespace GameSolver {
namespace Connect4 {

inline std::function<void(int64_t)> progress_handler;
inline int64_t progress_every = 1;

inline void report_progress(int64_t done) {
  if(progress_handler && done % progress_every == 0) progress_handler(done);
}

} // namespace Connect4
} // namespace GameSolver
#endif
