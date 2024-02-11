#ifndef TRACK_HPP
#define TRACK_HPP

#include <SDL2/SDL_mixer.h>
#include <chrono>
#include <string>

class Track {
public:
  std::string name, artist, album, path;
  int duration;

  bool isPlaying = false;

  Mix_Music* music;

  std::string formattedDuration(int durationSeconds);

  Track(std::string name, std::string artist, std::string album, std::string path, int duration) : name(name), artist(artist), album(album), path(path), duration(duration) {}

  void loadMusic();
  void startTimer();
  void stopTimer();
  void toggle();
  void forward();
  void backwards();
};

#endif
