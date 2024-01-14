#include "include/track.hpp"
#include <sstream>
#include <iomanip>

std::string Track::formattedDuration() {
  int seconds = duration % 60;
  int minutes = (duration / 60) % 60;
  int hours = duration / 3600;

  std::stringstream stream;

  if (hours) stream << std::setw(2) << std::setfill('0') << hours << ":";

  if (minutes) stream << std::setw(2) << std::setfill('0') << minutes << ":";

  stream << std::setw(2) << std::setfill('0') << seconds;

  return stream.str();
}
