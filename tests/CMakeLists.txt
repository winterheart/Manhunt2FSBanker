add_executable(SpeechLst_tests
  SpeechLst_tests.cpp
)
target_link_libraries(SpeechLst_tests PRIVATE File GTest::gtest_main)
gtest_discover_tests(SpeechLst_tests
  WORKING_DIRECTORY ${MANHUNT2_INSTALL_PATH}
)

add_executable(ContextMapBin_tests
  ContextMapBin_tests.cpp
  utils.h
)
target_link_libraries(ContextMapBin_tests PRIVATE File GTest::gtest_main)
gtest_discover_tests(ContextMapBin_tests
  WORKING_DIRECTORY ${MANHUNT2_INSTALL_PATH}
)

add_test(NAME FSB_Extract_Scripted
  COMMAND manhunt2fsbanker unpack ${MANHUNT2_INSTALL_PATH}/audio/PC/Scripted.fsb extract
)

add_test(NAME FSB_Pack_Scripted
  COMMAND manhunt2fsbanker pack extract/Scripted extract/
)
set_tests_properties(FSB_Pack_Scripted PROPERTIES
  FIXTURES_REQUIRED FSB_Extract_Scripted
)

add_test(FSB_Compare_Scripted
  ${CMAKE_COMMAND} -E compare_files ${MANHUNT2_INSTALL_PATH}/audio/PC/Scripted.fsb extract/Scripted.fsb
)
set_tests_properties(FSB_Compare_Scripted PROPERTIES
  FIXTURES_REQUIRED FSB_Pack_Scripted
  FIXTURES_CLEANUP FSB_Cleanup
)

add_test(NAME FSB_Extract_Executions
  COMMAND manhunt2fsbanker unpack ${MANHUNT2_INSTALL_PATH}/audio/PC/Executions.fsb extract
)

add_test(NAME FSB_Pack_Executions
  COMMAND manhunt2fsbanker pack extract/Executions extract/
)
set_tests_properties(FSB_Pack_Executions PROPERTIES
  FIXTURES_REQUIRED FSB_Extract_Executions
)

add_test(FSB_Compare_Executions
  ${CMAKE_COMMAND} -E compare_files ${MANHUNT2_INSTALL_PATH}/audio/PC/Executions.fsb extract/Executions.fsb
)
set_tests_properties(FSB_Compare_Executions PROPERTIES
  FIXTURES_REQUIRED FSB_Pack_Executions
  FIXTURES_CLEANUP FSB_Cleanup
)

add_test(NAME FSB_Extract_Speech_A06_CIA_Trap
  COMMAND manhunt2fsbanker unpack ${MANHUNT2_INSTALL_PATH}/audio/PC/Speech/A06_CIA_Trap.fsb extract
)
add_test(NAME FSB_Pack_Speech_A06_CIA_Trap
  COMMAND manhunt2fsbanker pack extract/A06_CIA_Trap extract/
)
set_tests_properties(FSB_Pack_Speech_A06_CIA_Trap PROPERTIES
  FIXTURES_REQUIRED FSB_Extract_Speech_A06_CIA_Trap
)

add_test(FSB_Compare_Speech_A06_CIA_Trap
  ${CMAKE_COMMAND} -E compare_files ${MANHUNT2_INSTALL_PATH}/audio/PC/Speech/A06_CIA_Trap.fsb extract/A06_CIA_Trap.fsb
)
set_tests_properties(FSB_Compare_Speech_A06_CIA_Trap PROPERTIES
  FIXTURES_REQUIRED FSB_Pack_Speech_A06_CIA_Trap
  FIXTURES_CLEANUP FSB_Cleanup
)

add_test(NAME FSB_Cleanup
  COMMAND ${CMAKE_COMMAND} -E remove_directory extract
)
