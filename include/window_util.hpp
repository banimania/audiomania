#ifndef WINDOW_UTIL_HPP
#define WINDOW_UTIL_HPP

#include <ncurses.h>
#include <string>

void clearWindow(WINDOW* win);
void clearLine(WINDOW* win, int line);

#endif
