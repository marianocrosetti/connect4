// Progress bar of c4: shows the reports of the library (report_progress in Progress.hpp) with indicators.
//   c4::progress::start("enumerate", total);   // creates the bar and starts listening
//   ... library code calls report_progress(done) ...
//   c4::progress::finish();                    // fills the bar to 100% and stops listening
// One bar at a time. total <= 0 (unknown): no bar.
#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "Progress.hpp"
#include "indicators.hpp"

namespace c4::progress {

inline std::unique_ptr<indicators::ProgressBar> bar;
inline int64_t bar_total = 0;

inline void start(const std::string& prefix, int64_t total) {
  if (total <= 0) return;
  bar_total = total;
  using namespace indicators;
  bar = std::make_unique<ProgressBar>(
      option::PrefixText{prefix + " "}, option::BarWidth{30}, option::Start{"["}, option::End{"]"},
      option::Fill{"#"}, option::Lead{"#"}, option::Remainder{"."}, option::MaxProgress{size_t(total)},
      option::ShowPercentage{true}, option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
      option::Stream{std::cerr});
  GameSolver::Connect4::progress_handler = [](int64_t done) { bar->set_progress(done); };
  GameSolver::Connect4::progress_every = total / 1000 > 0 ? total / 1000 : 1;
}

inline void finish() {
  GameSolver::Connect4::progress_handler = nullptr;
  GameSolver::Connect4::progress_every = 1;
  if (!bar) return;
  if (!bar->is_completed()) bar->set_progress(bar_total);
  bar.reset();
}

}  // namespace c4::progress
