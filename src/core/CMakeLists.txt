cmake_minimum_required(VERSION 3.16.0)

file(GLOB ENKIDU_CORE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB ENKIDU_CORE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(enkidu-core
	"${ENKIDU_CORE_SOURCE}"
	"${ENKIDU_CORE_INCLUDES}"
)

target_include_directories(enkidu-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(enkidu-core PRIVATE "/usr/local/include")

set_target_properties(enkidu-core PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

if(ENKIDU_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
