project(hankbd)

add_library(hankbd STATIC hankbd.c)
add_executable(sample main.c)
target_link_libraries(sample hankbd)

add_library(han STATIC han.c)
add_executable(sample2 han_main.c)
target_link_libraries(sample2 han)

add_library(limited_han STATIC limited_han.c)
add_executable(sample3 han_main.c)
target_link_libraries(sample3 limited_han)

enable_testing()
add_executable(test test.c)
target_link_libraries(test hankbd)
add_test(NAME test COMMAND ./test)
