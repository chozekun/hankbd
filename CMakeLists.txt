project(hankbd)

add_library(hankbd STATIC hankbd.c)

add_executable(sample main.c)

target_link_libraries(sample hankbd)

enable_testing()
add_executable(test test.c)
target_link_libraries(test hankbd)
add_test(NAME test COMMAND ./test)
