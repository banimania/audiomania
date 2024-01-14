#ifndef TRACK_HPP
#define TRACK_HPP

#include <string>

class Track {
public:
  std::string name, artist, path;
  int duration;

  std::string formattedDuration();

  Track(std::string name, std::string artist, std::string path, int duration) : name(name), artist(artist), path(path), duration(duration) {}
};

#endif
