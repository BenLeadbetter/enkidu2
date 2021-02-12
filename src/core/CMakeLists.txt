cmake_minimum_required(VERSION 3.16.0)

set(ENKIDU_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(ENKIDU_CORE_SOURCE
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/Store.cpp"
)

set(ENKIDU_CORE_INCLUDES
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Model.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/NodeMatrix.hpp"

    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Action.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Reducer.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Selector.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Store.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Subscription.hpp"
)

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
