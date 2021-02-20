cmake_minimum_required(VERSION 3.16.0)

project(LineNoise LANGUAGES C)

option(LINENOISE_EXAMPLE "Build Linenoise example app" OFF)

add_library(linenoise "src/linenoise.c")
target_include_directories(linenoise PUBLIC "include")

if(LINENOISE_EXAMPLE)
	add_subdirectory(example)
endif()

