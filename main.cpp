#include <ncurses.h>
#include <string>
#include <vector>
#include "include/window_util.hpp"
#include "include/track.hpp"

void draw(WINDOW* winTrackSel, WINDOW* winTrack, std::vector<Track*> tracks, Track* currentTrack, int selectedOption, int auxx, int auxy) {
  refresh();
  box(winTrackSel, 0, 0);
  box(winTrack, 0, 0);
  
  wattron(winTrackSel, COLOR_PAIR(2));
  wattron(winTrackSel, A_BOLD);
  mvwprintw(winTrackSel, 0, auxx / 2 - std::string(" Select Track ").size() / 2, " Select Track ");
  wattroff(winTrackSel, A_BOLD);
  wattroff(winTrackSel, COLOR_PAIR(2));

  int nx = 2, ntrack = 5, nartist = 40, nduration = 75;
  mvwprintw(winTrackSel, 1, nx, "#");
  mvwprintw(winTrackSel, 1, ntrack, "Track");
  mvwprintw(winTrackSel, 1, nartist, "Artist");
  mvwprintw(winTrackSel, 1, nduration, "Duration");
  for (int i = 0; i < tracks.size(); i++) {
    if (selectedOption == i) wattron(winTrackSel, COLOR_PAIR(3));
    std::string line = std::string(" " + std::to_string(i + 1) + ". " + tracks[i]->name);
    int s = line.size();
    for (int j = 0; j < nartist - s - 1; j++) line += " ";
    line += tracks[i]->artist;
    s = line.size();
    for (int k = 0; k < nduration - s - 1; k++) line += " ";
    line += tracks[i]->formattedDuration();
    s = line.size();
    for (int l = 0; l < nduration + std::string("Duration").size() - s - 1; l++) line += " ";
    mvwprintw(winTrackSel, 2 + i, 1, line.c_str(), auxx);
    wattroff(winTrackSel, COLOR_PAIR(3));
  }
  
  wattron(winTrack, COLOR_PAIR(2));
  wattron(winTrack, A_BOLD);
  mvwprintw(winTrack, 0, auxx / 2 - std::string(" Track Information ").size() / 2, " Track Information ");
  wattroff(winTrack, A_BOLD);
  wattroff(winTrack, COLOR_PAIR(2));

  clearWindow(winTrack);
  if (!currentTrack) {
    wattron(winTrack, COLOR_PAIR(4));
    mvwprintw(winTrack, 1, 2, "No current track selected!");
    wattroff(winTrack, COLOR_PAIR(4));
  } else {
    wattron(winTrack, COLOR_PAIR(1));
    mvwprintw(winTrack, 1, 2, std::string("Track: " + currentTrack->name).c_str());
    mvwprintw(winTrack, 2, 2, std::string("Artist: " + currentTrack->artist).c_str());
    mvwprintw(winTrack, 3, 2, std::string("Duration: " + currentTrack->formattedDuration()).c_str());
    wattroff(winTrack, COLOR_PAIR(1));
  }

  attron(COLOR_PAIR(1));
  attron(A_BOLD);
  mvwprintw(stdscr, 0, auxx / 2 - std::string("AudioMania").size() / 2, "AudioMania");
  
  wrefresh(winTrackSel);
  wrefresh(winTrack);
  refresh();
}

int main(int argc, char* argv[]) {
  Track* currentTrack = NULL;
  std::vector<Track*> tracks;
  tracks.push_back(new Track("Track a", "author1", "pstra.mp3", 253));
  tracks.push_back(new Track("Track b", "author2", "pstra.mp3", 318));

  int selectedOption = 0;

  initscr();
  keypad(stdscr, true);

  curs_set(0);
  nodelay(stdscr, true);

  use_default_colors();

  start_color();
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_CYAN, -1);
  init_pair(3, COLOR_BLACK, COLOR_WHITE);
  init_pair(4, COLOR_RED, -1);

  int auxx, auxy;
  getmaxyx(stdscr, auxy, auxx);
  --auxy;
  auxy /= 3;

  WINDOW* winTrackSel = newwin(2 * auxy, auxx, 1, 0);
  WINDOW* winTrack = newwin(auxy, 0, 2 * auxy + 1, 0);

  char ch;
  while ((ch = getch()) != 27) {
    switch (ch) {
      case 2:
        selectedOption++;
        break;
      case 3:
        selectedOption--;
        break;
      case 10:
        currentTrack = tracks[selectedOption];
        break;
    }
    if (selectedOption < 0) selectedOption = tracks.size() - 1;
    else selectedOption %= tracks.size();

    //mvwprintw(stdscr, 0, 0, std::to_string(ch).c_str());

    draw(winTrackSel, winTrack, tracks, currentTrack, selectedOption, auxx, auxy);
  };
  
  endwin();
  return 0;
}
