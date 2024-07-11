project(hankbd)

add_library(hankbd STATIC hankbd.c)
add_library(han STATIC han.c)
add_library(limited_han STATIC han.c limited_han.c)

add_executable(main_hankbd main_hankbd.c)
target_link_libraries(main_hankbd hankbd)

add_executable(main_han main_han.c)
target_link_libraries(main_han han)

add_executable(main_limited_han main_limited_han.c)
target_link_libraries(main_limited_han limited_han)

enable_testing()
add_executable(test_hankbd test_hankbd.c)
target_link_libraries(test_hankbd hankbd)
add_test(NAME test_hankbd COMMAND ./test_hankbd)

add_executable(test_limited_han test_limited_han.c)
target_link_libraries(test_limited_han limited_han)
add_test(NAME test_limited_han COMMAND ./test_limited_han)
