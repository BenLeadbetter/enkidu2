cmake_minimum_required(VERSION 3.16.0)

set(ENKIDU_HEADLESS_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/Application.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/Input.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/OnError.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/Parser.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/Show.cpp"
)

set(ENKIDU_HEADLESS_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/Application.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/Command.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/DispatchCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/Input.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/OnError.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/Parser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/app/Show.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/model/Action.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/model/SideEffect.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/headless/model/Store.hpp"
)

add_library(enkidu-headless-lib
        "src/main.cpp"
	${ENKIDU_HEADLESS_SOURCE}
	${ENKIDU_HEADLESS_INCLUDES}
)

target_link_libraries(enkidu-headless-lib
        PUBLIC enkidu-core
        PUBLIC linenoise
        PUBLIC CLI11
)

target_include_directories(enkidu-headless-lib PUBLIC "include")

set_target_properties(enkidu-headless-lib PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

add_executable(enkidu-headless "src/main.cpp")
target_link_libraries(enkidu-headless PRIVATE enkidu-headless-lib)
set_target_properties(enkidu-headless PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if(ENKIDU_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
