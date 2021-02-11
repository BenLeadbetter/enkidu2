cmake_minimum_required(VERSION 3.16.0)

find_package(Qt5
    COMPONENTS
        Core REQUIRED)

set(ENKIDU_HEADLESS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB ENKIDU_HEADLESS_SOURCE "${ENKIDU_HEADLESS_SOURCE_DIR}/src/*.cpp")
file(GLOB ENKIDU_HEADLESS_INCLUDES "${ENKIDU_HEADLESS_SOURCE_DIR}/include/*.hpp")

add_executable(enkidu-headless
        "${ENKIDU_HEADLESS_SOURCE_DIR}/main.cpp"
	${ENKIDU_HEADLESS_SOURCE}
	${ENKIDU_HEADLESS_INCLUDES}
)

target_link_libraries(enkidu-headless
	PRIVATE enkidu-core
        PRIVATE Qt5::Core
)

target_include_directories(enkidu-headless PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(enkidu-headless PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

if(ENKIDU_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
