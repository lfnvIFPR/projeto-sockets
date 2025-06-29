cmake_minimum_required(VERSION 3.22)

project(
  "sockets"
  VERSION "0.0.1"
  DESCRIPTION "socket server and client example"
  LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("include")
add_subdirectory("src")