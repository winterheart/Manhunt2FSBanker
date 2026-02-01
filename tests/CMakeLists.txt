add_executable(SpeechLst_tests
  SpeechLst_tests.cpp
)
target_link_libraries(SpeechLst_tests PRIVATE File GTest::gtest_main)

gtest_discover_tests(SpeechLst_tests
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
)
