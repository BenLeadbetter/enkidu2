cmake_minimum_required(VERSION 3.16.0)

find_package(Catch2 REQUIRED)

add_executable(enkidu-core-tests
    "${CMAKE_CURRENT_SOURCE_DIR}/ModelTest.cpp"
)

target_link_libraries(enkidu-core-tests
	PRIVATE Catch2::Catch2
	PRIVATE enkidu-core
)

target_include_directories(enkidu-core-tests
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_target_properties(enkidu-core-tests
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES)
