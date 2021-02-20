cmake_minimum_required(VERSION 3.16.0)

project(Enkidu VERSION 0.1.0)

option(ENKIDU_TESTS "Enables tests" OFF)

add_subdirectory(src)
add_subdirectory(libs)

