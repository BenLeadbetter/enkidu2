cmake_minimum_required(VERSION 3.16.0)

find_package(Catch2 REQUIRED)

add_executable(enkidu-headless-tests ApplicationTest.cpp)

target_link_libraries(enkidu-headless-tests
	PRIVATE Catch2::Catch2
	PRIVATE enkidu-core)

target_include_directories(enkidu-headless-tests 
	PRIVATE "${ENKIDU_HEADLESS_SOURCE_DIR}/include")
