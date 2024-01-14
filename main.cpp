#include <ncurses.h>
#include <string>
#include <vector>
#include "include/window_util.hpp"
#include "include/track.hpp"

void drawTitle() {
  attron(COLOR_PAIR(1));
  attron(A_BOLD);
  mvwprintw(stdscr, 0, getmaxx(stdscr) / 2 - std::string("AudioMania").size() / 2, "AudioMania");
  attroff(A_BOLD);
  attroff(COLOR_PAIR(1));
}

void draw_track_select(WINDOW* window, std::vector<Track*> tracks, int selectedOption) {
  int maxx, maxy;
  getmaxyx(window, maxy, maxx);

  refresh();
  box(window, 0, 0);

  clearWindow(window);
  int nx = 2, ntrack = 5, nartist = 2 * maxx / 5, nduration =4 * maxx / 5;
  mvwprintw(window, 1, nx, "#");
  mvwprintw(window, 1, ntrack, "Track");
  mvwprintw(window, 1, nartist, "Artist");
  mvwprintw(window, 1, nduration, "Duration");
  for (int i = 0; i < tracks.size(); i++) {
    if (selectedOption == i) wattron(window, COLOR_PAIR(3));
    std::string line = std::string(" " + std::to_string(i + 1) + ". " + tracks[i]->name);
    int s = line.size();
    for (int j = 0; j < nartist - s - 1; j++) line += " ";
    line += tracks[i]->artist;
    s = line.size();
    for (int j = 0; j < nduration - s - 1; j++) line += " ";
    line += tracks[i]->formattedDuration();
    s = line.size();
    for (int j = 0; j < nduration + std::string("Duration").size() - s - 1; j++) line += " ";
    s = line.size();
    for (int j = 0; j < maxx - s - 2; j++) line += " ";
    mvwprintw(window, 2 + i, 1, line.c_str(), maxx);
    wattroff(window, COLOR_PAIR(3));
  }

  wattron(window, COLOR_PAIR(2));
  wattron(window, A_BOLD);
  mvwprintw(window, 0, maxx / 2 - std::string(" Select Track ").size() / 2, " Select Track ");
  wattroff(window, A_BOLD);
  wattroff(window, COLOR_PAIR(2));

  wrefresh(window);
  refresh();
}

void draw_track_info(WINDOW* window, Track* currentTrack) {
  int maxx, maxy;
  getmaxyx(window, maxy, maxx);

  refresh();
  box(window, 0, 0);

  clearWindow(window);

  wattron(window, COLOR_PAIR(2));
  wattron(window, A_BOLD);
  mvwprintw(window, 0, maxx / 2 - std::string(" Current Track ").size() / 2, " Current Track ");
  wattroff(window, A_BOLD);
  wattroff(window, COLOR_PAIR(2));

  if (!currentTrack) {
    wattron(window, COLOR_PAIR(4));
    wattron(window, A_BOLD);
    mvwprintw(window, maxy / 2 - 1, maxx / 2 - std::string("No track selected yet!").size() / 2, "No track selected yet!");
    wattroff(window, A_BOLD);
    wattroff(window, COLOR_PAIR(4));
  } else {
    wattron(window, COLOR_PAIR(1));
    wattron(window, A_BOLD);
    std::string name = std::string("Name: " + currentTrack->name);
    std::string artist = std::string("Artist: " + currentTrack->artist);
    std::string duration = std::string("Duration: " + currentTrack->formattedDuration());
    mvwprintw(window, maxy / 2 - 3, maxx / 2 - name.size() / 2, name.c_str());
    mvwprintw(window, maxy / 2 - 2, maxx / 2 - artist.size() / 2, artist.c_str());
    mvwprintw(window, maxy / 2 - 1, maxx / 2 - duration.size() / 2, duration.c_str());
    wattroff(window, A_BOLD);
    wattroff(window, COLOR_PAIR(1));
  }

  wrefresh(window);
  refresh();
}

void draw_sound_visualizer(WINDOW* window) {
  int maxx, maxy;
  getmaxyx(window, maxy, maxx);

  refresh();
  box(window, 0, 0);

  clearWindow(window);

  wattron(window, COLOR_PAIR(2));
  wattron(window, A_BOLD);
  mvwprintw(window, 0, maxx / 2 - std::string(" Sound Visualizer ").size() / 2, " Sound Visualizer ");
  wattroff(window, A_BOLD);
  wattroff(window, COLOR_PAIR(2));

  wrefresh(window);
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

  WINDOW* winTrackSel = newwin(2 * auxy, (auxx / 2), 1, 0);
  WINDOW* winTrack = newwin(auxy, 0, 2 * auxy + 1, 0);
  WINDOW* winSound = newwin(2 * auxy, (auxx / 2), 1, (auxx / 2));

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

    draw_track_select(winTrackSel, tracks, selectedOption);
    draw_track_info(winTrack, currentTrack);
    draw_sound_visualizer(winSound);
    drawTitle();
  };

  endwin();
  return 0;
}
