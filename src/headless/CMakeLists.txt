cmake_minimum_required(VERSION 3.16.0)

set(ENKIDU_HEADLESS_SOURCE
	"src/main.cpp"

        "src/app/Application.cpp"
        "src/app/Input.cpp"
)

set(ENKIDU_HEADLESS_INCLUDES
    "include/headless/app/Application.hpp"
    "include/headless/app/Command.hpp"
    "include/headless/app/DispatchCommand.hpp"
    "include/headless/app/Input.hpp"

    "include/headless/model/Action.hpp"
    "include/headless/model/SideEffect.hpp"
    "include/headless/model/Store.hpp"
)

add_executable(enkidu-headless
	${ENKIDU_HEADLESS_SOURCE}
	${ENKIDU_HEADLESS_INCLUDES}
)

target_link_libraries(enkidu-headless
	PRIVATE enkidu-core
        PRIVATE linenoise
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
