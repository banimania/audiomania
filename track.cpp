#include "include/track.hpp"
#include <SDL2/SDL_mixer.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

std::string Track::formattedDuration(int durationSeconds) {
  int seconds = durationSeconds % 60;
  int minutes = (durationSeconds / 60) % 60;
  int hours = durationSeconds / 3600;

  std::stringstream stream;

  if (hours) stream << std::setw(2) << std::setfill('0') << hours << ":";

  stream << std::setw(2) << std::setfill('0') << minutes << ":";

  stream << std::setw(2) << std::setfill('0') << seconds;

  return stream.str();
}

void Track::loadMusic() {
  music = Mix_LoadMUS(path.c_str());
}

void Track::startTimer() {
  if (Mix_GetMusicPosition(music) == 0) {
    Mix_PlayMusic(music, 0);
  }
  else Mix_ResumeMusic();
  isPlaying = true;
}

void Track::stopTimer() {
  Mix_PauseMusic();
  isPlaying = false;
}

void Track::toggle() {
  isPlaying ? stopTimer() : startTimer();
}

void Track::forward() {
  float newPos = std::min(Mix_GetMusicPosition(music) + 5.0f, Mix_MusicDuration(music));
  Mix_SetMusicPosition(newPos);
}

void Track::backwards() {
  float offset = std::min(5.0f, (float) Mix_GetMusicPosition(music));
  Mix_SetMusicPosition(Mix_GetMusicPosition(music) - offset);
}
