#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_timer.h>
#include <clocale>
#include <cstring>
#include <filesystem>
#include <ncurses.h>
#include <string>
#include <taglib/audioproperties.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "include/window_util.hpp"
#include "include/track.hpp"

#include <taglib/tag.h>
#include <taglib/tvariant.h>
#include <taglib/fileref.h>

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
    line += tracks[i]->formattedDuration(tracks[i]->duration);
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

  std::string enter = " [Enter] Select ";
  
  if (!currentTrack) {
    wattron(window, COLOR_PAIR(4));
    wattron(window, A_BOLD);
    mvwprintw(window, maxy / 2, maxx / 2 - std::string("No track selected yet!").size() / 2, "No track selected yet!");
    wattroff(window, A_BOLD);
    wattroff(window, COLOR_PAIR(4));
    
    wattron(window, COLOR_PAIR(3));
    mvwprintw(window, (int) (maxy - maxy / 3.0f) + 1, maxx / 2.0f - strlen(enter.c_str()) / 2.0f, enter.c_str());
    wattroff(window, COLOR_PAIR(3));
  } else {
    wattron(window, COLOR_PAIR(1));
    wattron(window, A_BOLD);
    std::string name = currentTrack->name + " - " + currentTrack->artist;
    if (currentTrack->album != currentTrack->name && currentTrack->album != currentTrack->artist && !currentTrack->album.empty()) name += " - " + currentTrack->album;
    std::string duration = std::string(currentTrack->formattedDuration(Mix_GetMusicPosition(currentTrack->music)) + " / " + currentTrack->formattedDuration(currentTrack->duration));
    mvwprintw(window, maxy - maxy / 3.0f - 4, maxx / 2 - name.size() / 2, name.c_str());
    mvwprintw(window, maxy - maxy / 3.0f - 2, maxx / 2 - duration.size() / 2, duration.c_str());
    wattroff(window, A_BOLD);
    wattroff(window, COLOR_PAIR(1));

    std::string bar = "[";
    float percentage = (float) Mix_GetMusicPosition(currentTrack->music) / (float) currentTrack->duration;
    for (int i = 0; i < maxx - maxx / 3.0f; i++) {
      if (i == (int) (maxx - maxx / 3.0f) - 1) bar += "]";
      else bar += "-";
    }
    mvwprintw(window, maxy - maxy / 3.0f - 1, maxx / 6.0f, bar.c_str()); 
    mvwprintw(window, maxy - maxy / 3.0f - 1, maxx / 6.0f + 1 + (percentage * (maxx - maxx / 3.0f - 2)), "|");
  
    std::string space = currentTrack->isPlaying ? " [Space] Pause " : " [Space] Resume ";

    wattron(window, COLOR_PAIR(3));
    mvwprintw(window, (int) (maxy - maxy / 3.0f) + 1, maxx / 2.0f - strlen(enter.c_str()), enter.c_str());
    mvwprintw(window, (int) (maxy - maxy / 3.0f) + 1, maxx / 2.0f + 1, space.c_str());
    wattroff(window, COLOR_PAIR(3));
  }

  wrefresh(window);
  refresh();
}

void load_music(std::vector<Track*>* tracks) {
  for (const auto &m : std::filesystem::directory_iterator("music")) {
    std::string name = std::string(m.path());
    name = name.substr(name.find("/") + 1, name.length());
    name = name.substr(0, name.find("."));

    TagLib::FileRef f = TagLib::FileRef(m.path().c_str());
    if (!f.isNull() && f.tag()) {
      TagLib::Tag* tag = f.tag();
      TagLib::AudioProperties* properties = f.audioProperties();
      
      tracks->push_back(new Track(tag->title().to8Bit(), tag->artist().to8Bit(), tag->album().to8Bit(), m.path(), properties->lengthInSeconds()));
    }
  }
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "es_ES.UTF-8");

  SDL_Init(SDL_INIT_AUDIO);
  Mix_Init(MIX_INIT_MP3);
  Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);

  Track* currentTrack = NULL;
  std::vector<Track*> tracks;
  load_music(&tracks);

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
  auxy /= 6;

  WINDOW* winTrackSel = newwin(5 * auxy, auxx, 1, 0);
  WINDOW* winTrack = newwin(auxy + 1, 0, 5 * auxy + 1, 0);

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
        if (currentTrack != NULL) {
          if (currentTrack == tracks[selectedOption]) break;
          else {
            currentTrack->stopTimer();
            Mix_SetMusicPosition(0);
          }
        }
        currentTrack = tracks[selectedOption];
        currentTrack->loadMusic();
        currentTrack->startTimer();
        break;
      case 32:
        if (currentTrack) currentTrack->toggle();
        break;
      case 5:
        currentTrack->forward();
        break;
      case 4:
        currentTrack->backwards();
        break;
    }
    if (selectedOption < 0) selectedOption = tracks.size() - 1;
    else selectedOption %= tracks.size();

    draw_track_select(winTrackSel, tracks, selectedOption);
    draw_track_info(winTrack, currentTrack);
    drawTitle();
  };

  SDL_Quit();
  
  endwin();
  return 0;
}
