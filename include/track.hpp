#ifndef TRACK_HPP
#define TRACK_HPP

#include <chrono>
#include <string>

class Track {
public:
  std::string name, artist, album, path;
  int duration;
  int currentSecond = 0.0f;

  bool isPlaying = false;

  std::chrono::time_point<std::chrono::system_clock> first;

  std::string formattedDuration(int durationSeconds);

  Track(std::string name, std::string artist, std::string album, std::string path, int duration) : name(name), artist(artist), album(album), path(path), duration(duration) {}
  
  void startTimer() {
    isPlaying = true;
    first = std::chrono::system_clock::now();
  }

  void updateTimer() {
    if (!isPlaying) return;
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    currentSecond = std::chrono::duration_cast<std::chrono::seconds>(now - first).count();
  }

  void stopTimer() {
    updateTimer();
    isPlaying = false;
  }
};

#endif
