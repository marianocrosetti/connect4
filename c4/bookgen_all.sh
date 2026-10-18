#!/usr/bin/env bash
# Generates several books at the same time.
# Usage: ./bookgen_all.sh
# tmux:
#   - Ctrl-b w: list windows
#   - Ctrl-b n / p: next / previous window
#   - Ctrl-b d: detach (runs keep going)
#   - tmux attach -t books: attach again
tmux new-session -d -s books
for r in V H VH D1 D2 VHD2 VHD1D2; do
  tmux new-window -t books -n $r "./build/c4 bookgen --rules $r --depth 8 --out books/6x5; read"
done
tmux attach -t books
