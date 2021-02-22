cmake_minimum_required(VERSION 3.16.0)

find_package(Catch2 REQUIRED)

add_executable(enkidu-headless-tests
    ParserTest.cpp
)

target_link_libraries(enkidu-headless-tests
	PRIVATE Catch2::Catch2
        PRIVATE enkidu-core
        PRIVATE enkidu-headless-lib)

target_include_directories(enkidu-headless-tests PRIVATE "${ENKIDU_HEADLESS_INCLUDE_DIR}")

set_target_properties(enkidu-headless-tests
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES)
