cmake_minimum_required(VERSION 3.16.0)

set(ENKIDU_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(ENKIDU_CORE_INCLUDE_DIR "${ENKIDU_CORE_INCLUDE_DIR}/include")

find_package(Immer REQUIRED)
find_package(Eigen3 REQUIRED)

set(ENKIDU_CORE_SOURCE
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/Node.cpp"
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/Reducers.cpp"
    "${ENKIDU_CORE_SOURCE_DIR}/src/model/ReducerHelpers.cpp"
)

set(ENKIDU_CORE_INCLUDES
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Action.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Actions.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Connections.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Id.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Model.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Node.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/Reducers.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/ReducerHelpers.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/SideEffect.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/model/SideEffects.hpp"

    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/udf/Action.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/udf/Reducer.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/udf/Reduction.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/udf/Selector.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/udf/SideEffect.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/udf/Store.hpp"

    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/utility/Assert.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/utility/is_one_of.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/utility/flatten_variants.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/utility/overload.hpp"
    "${ENKIDU_CORE_SOURCE_DIR}/include/enkidu/utility/registry.hpp"
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
