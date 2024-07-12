project(hankbd)

add_library(hankbd STATIC hankbd.c)
add_library(han STATIC han.c)

add_executable(main_hankbd main_hankbd.c)
target_link_libraries(main_hankbd hankbd)

add_executable(main_han main_han.c)
target_link_libraries(main_han han)

add_executable(main_limited_han main_limited_han.c)
target_link_libraries(main_limited_han han)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
include(CodeCoverage)
append_coverage_compiler_flags()

enable_testing()
add_executable(test_hankbd test_hankbd.c)
target_link_libraries(test_hankbd hankbd)
add_test(NAME test_hankbd COMMAND ./test_hankbd)

add_executable(test_han test_han.c)
target_link_libraries(test_han han)
add_test(NAME test_han COMMAND ./test_han)

add_executable(test_limited_han test_limited_han.c)
target_link_libraries(test_limited_han han)
add_test(NAME test_limited_han COMMAND ./test_limited_han)

setup_target_for_coverage_lcov(
  NAME coverage
  EXECUTABLE ctest
  DEPENDENCIES
	  test_hankbd
	  test_han
	  test_limited_han
)
