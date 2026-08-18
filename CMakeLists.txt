cmake_minimum_required(VERSION 3.20)
project(Manhunt2FSBanker VERSION 0.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MANHUNT2_INSTALL_PATH "Manhunt 2 installation path (required for testing)")

find_package(yaml-cpp CONFIG REQUIRED)

configure_file(manhunt2fsbanker_version.h.in manhunt2fsbanker_version.h)

add_library(File
  File/ContextMapBin.cpp
  File/CRC32.cpp
  File/Dir.cpp
  File/DirNames.cpp
  File/Fsb.cpp
  File/SpeechLst.cpp
  File/Wav.cpp
  File/WavCodec.cpp
  File/YAMLHelpers.cpp
)
target_include_directories(File PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/File>
)
target_link_libraries(File PUBLIC
  yaml-cpp::yaml-cpp
)

add_executable(manhunt2fsbanker
  main.cpp
)
target_include_directories(manhunt2fsbanker PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(manhunt2fsbanker PRIVATE
  File
)
install(TARGETS manhunt2fsbanker RUNTIME DESTINATION .)
install(FILES README.md DESTINATION .)
if(BUILD_TESTING)
  if(NOT IS_DIRECTORY ${MANHUNT2_INSTALL_PATH})
    message(FATAL_ERROR "MANHUNT2_INSTALL_PATH does not exists!")
  endif()
  message(STATUS "Samples dir: ${MANHUNT2_INSTALL_PATH}")
  find_package(GTest REQUIRED)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()

# CPack configuration
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
include(CPack)

