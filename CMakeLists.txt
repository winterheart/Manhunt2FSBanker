cmake_minimum_required(VERSION 3.20)
project(Manhunt2FSBanker VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(manhunt2fsbanker_version.h.in manhunt2fsbanker_version.h)

add_executable(manhunt2fsbanker
  main.cpp
  File/FileContainer.cpp
)
target_include_directories(manhunt2fsbanker PUBLIC	"${PROJECT_BINARY_DIR}")
target_link_options(manhunt2fsbanker PRIVATE
  $<$<PLATFORM_ID:Windows>:-static>
  $<$<PLATFORM_ID:Windows>:-s>
)
