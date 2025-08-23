cmake_minimum_required(VERSION 3.27)
project(Raytracing)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR
    "Ninja"
    CACHE INTERNAL "Ninja" FORCE)
set(CMAKE_CXX_COMPILER
    "clang++"
    CACHE INTERNAL "clang++" FORCE)

file(GLOB SRC_NORMAL "./src/*.cpp")
file(GLOB SRC_NORMAL_NO_MAIN "./src/*.cpp")
list(FILTER SRC_NORMAL_NO_MAIN EXCLUDE REGEX ".*main.cpp")
set(INCLUDE_SELF "./include/Raytracing/")
set(IMG_IN "${CMAKE_SOURCE_DIR}/img/input")
set(IMG_OUT "${CMAKE_SOURCE_DIR}/img/output")
set(KawaiiMQ_BUILD_TESTS
    OFF
    CACHE BOOL "Build tests")
file(MAKE_DIRECTORY ${IMG_IN})
file(MAKE_DIRECTORY ${IMG_OUT})

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.x)
FetchContent_MakeAvailable(spdlog)
list(APPEND INCLUDE_THIRDPARTIES ${spdlog_SOURCE_DIR}/include)
link_libraries(spdlog::spdlog)

FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master)
FetchContent_MakeAvailable(eigen)
list(APPEND INCLUDE_THIRDPARTIES ${eigen_SOURCE_DIR})
link_libraries(Eigen3::Eigen)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG master)
FetchContent_MakeAvailable(stb)
list(APPEND INCLUDE_THIRDPARTIES ${stb_SOURCE_DIR})

FetchContent_Declare(
  gpucpp
  GIT_REPOSITORY https://github.com/AnswerDotAI/gpu.cpp.git
  GIT_TAG main)
FetchContent_MakeAvailable(gpucpp)
list(APPEND INCLUDE_THIRDPARTIES ${gpucpp_SOURCE_DIR})
link_libraries(webgpu_dawn)

FetchContent_Declare(
  KawaiiMQ
  GIT_REPOSITORY https://github.com/kagurazaka-ayano/KawaiiMq
  GIT_TAG main)
FetchContent_MakeAvailable(KawaiiMQ)
list(APPEND INCLUDE_SELF ${KawaiiMQ_SOURCE_DIR}/include)
link_libraries(KawaiiMQ)

set(INCLUDE ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})
include_directories(${INCLUDE})
add_executable(RaytracingNormal ${SRC_NORMAL})
add_executable(RaytracingAscii ${SRC_NORMAL})
target_compile_definitions(RaytracingAscii PUBLIC IMG_INPUT_DIR="${IMG_IN}")
target_compile_definitions(RaytracingNormal PUBLIC IMG_INPUT_DIR="${IMG_IN}")
target_compile_definitions(RaytracingAscii PUBLIC IMG_OUTPUT_DIR="${IMG_OUT}")
target_compile_definitions(RaytracingNormal PUBLIC IMG_OUTPUT_DIR="${IMG_OUT}")
target_compile_definitions(RaytracingAscii PUBLIC "ASCII_ART")
add_compile_definitions(CMAKE_EXPORT_COMPILE_COMMANDS=1)
set_target_properties(
  RaytracingNormal
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG
             "${CMAKE_SOURCE_DIR}/bin/normal/debug"
             RUNTIME_OUTPUT_DIRECTORY_RELEASE
             "${CMAKE_SOURCE_DIR}/bin/normal/release")
set_target_properties(
  RaytracingAscii
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG
             "${CMAKE_SOURCE_DIR}/bin/ascii/debug"
             RUNTIME_OUTPUT_DIRECTORY_RELEASE
             "${CMAKE_SOURCE_DIR}/bin/ascii/release")
