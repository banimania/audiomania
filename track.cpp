#include "include/track.hpp"
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
