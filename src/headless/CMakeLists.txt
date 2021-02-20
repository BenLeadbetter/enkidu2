cmake_minimum_required(VERSION 3.16.0)

set(ENKIDU_HEADLESS_SOURCE
	"src/main.cpp"
)

set(ENKIDU_HEADLESS_INCLUDES
)

add_executable(enkidu-headless
	${ENKIDU_HEADLESS_SOURCE}
	${ENKIDU_HEADLESS_INCLUDES}
)

target_link_libraries(enkidu-headless
	PRIVATE enkidu-core
	RPIVATE linenoise
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
