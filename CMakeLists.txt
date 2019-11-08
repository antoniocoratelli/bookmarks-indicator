project(bookmarks-indicator C CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

add_executable(bookmarks-indicator
  bookmarks-indicator.cpp)

target_include_directories(bookmarks-indicator
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE include)
target_link_directories(bookmarks-indicator
  PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(bookmarks-indicator
  PRIVATE ${GTKMM_LIBRARIES})
