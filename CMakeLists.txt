cmake_minimum_required (VERSION 2.8)
project (keithray)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-std=c++17")
enable_cxx_compiler_flag_if_supported("-fopenmp")

ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Creating the executable in the debug mode.")

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Creating the executable in the release mode.")

file(
    GLOB SOURCES
    "${SRC_DIR}/*.hpp"
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/models/materials/diffuse.cpp"
    "${SRC_DIR}/models/materials/specular.cpp"
    "${SRC_DIR}/models/materials/glossy.cpp"
    "${SRC_DIR}/models/materials/fresnel.cpp"
    "${SRC_DIR}/models/materials/material.cpp"
    "${SRC_DIR}/models/shapes/shape.cpp"
    "${SRC_DIR}/models/shapes/sphere.cpp"
    "${SRC_DIR}/models/cameras/camera.cpp"
    "${SRC_DIR}/models/cameras/pinhole.cpp"
    "${SRC_DIR}/models/cameras/lens-camera.cpp"
    "${SRC_DIR}/models/lights/light.cpp"
    "${SRC_DIR}/models/lights/point-light.cpp"
    "${SRC_DIR}/models/lights/area-light.cpp"
    "${SRC_DIR}/models/scene.cpp"
    "${SRC_DIR}/models/rays/ray-sampler.cpp"
    "${SRC_DIR}/renderers/renderer.cpp"
    "${SRC_DIR}/renderers/path-renderer/path-renderer.cpp"
    "${SRC_DIR}/renderers/whitted-renderer/whitted-renderer.cpp"
    "${SRC_DIR}/utils/utils.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

# JSON
# Add the library as a single-header interface
set(nlohmann_json_DIR "${LIB_DIR}/json")
add_subdirectory("${nlohmann_json_DIR}")
find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()