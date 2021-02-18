cmake_minimum_required(VERSION 3.16.0)

set(ENKIDU_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Immer REQUIRED)
find_package(Eigen3 REQUIRED)

set(ENKIDU_CORE_SOURCE
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/Node.cpp"
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/Reducers.cpp"
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/ReducerHelpers.cpp"
)

set(ENKIDU_CORE_INCLUDES
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Action.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Actions.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Connection.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Error.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Id.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Model.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Node.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/Reducers.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/ReducerHelpers.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/SideEffect.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/model/SideEffects.hpp"

    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Action.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Reducer.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Reduction.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Selector.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/SideEffect.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/udf/Store.hpp"

    "${ENKIDU_CORE_SOURCE_DIR}/include/utility/is_one_of.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/utility/flatten_variants.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/utility/overload.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/utility/registry.hpp"
)

add_library(enkidu-core
	"${ENKIDU_CORE_SOURCE}"
	"${ENKIDU_CORE_INCLUDES}"
)

target_include_directories(enkidu-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(enkidu-core
    PRIVATE Eigen3::Eigen)

set_target_properties(enkidu-core PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

if(ENKIDU_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
