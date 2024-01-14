#include "include/window_util.hpp"
#include <ncurses.h>

void clearWindow(WINDOW* win) {
  int h = getmaxy(win);

  for (int i = 1; i < h - 1; i++) clearLine(win, i);
}

void clearLine(WINDOW* win, int line) {
  int w = getmaxx(win);

  std::string clear;
  for (int i = 0; i < w - 2; i++) clear += " ";

  mvwprintw(win, line, 1, clear.c_str());
}
