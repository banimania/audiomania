cmake_minimum_required(VERSION 3.20.0)
project(audiomania)

include_directories(include)

add_executable(audiomania main.cpp track.cpp window_util.cpp)

target_link_libraries(audiomania ncurses)
