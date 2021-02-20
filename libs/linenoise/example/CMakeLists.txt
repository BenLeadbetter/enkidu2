cmake_minimum_required(VERSION 3.16.0)

add_executable(linenoise-example "example.c")
target_link_libraries(linenoise-example PRIVATE linenoise)
