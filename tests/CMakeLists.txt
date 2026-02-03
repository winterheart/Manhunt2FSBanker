add_executable(SpeechLst_tests
  SpeechLst_tests.cpp
)
target_link_libraries(SpeechLst_tests PRIVATE File GTest::gtest_main)
gtest_discover_tests(SpeechLst_tests
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
)

add_executable(ContextMapBin_tests
  ContextMapBin_tests.cpp
  utils.h
)
target_link_libraries(ContextMapBin_tests PRIVATE File GTest::gtest_main)
gtest_discover_tests(ContextMapBin_tests
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
)
