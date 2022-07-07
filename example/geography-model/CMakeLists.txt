cmake_minimum_required (VERSION 3.6.1)
project(pandemic-geographical_model)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_COMPILER "g++")
add_compile_options(-g)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

get_filename_component(cadmium "../../cadmium/json" REALPATH)

include_directories(../../../DESTimes/include)
include_directories(../../../cadmium/include)
include_directories(../../../cadmium/json/include)
include_directories(../../../cadmium-web-extension/include)

set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost COMPONENTS unit_test_framework system thread REQUIRED)

file(MAKE_DIRECTORY logs)

add_executable(geography-model main.cpp)

target_link_libraries(geography-model PUBLIC ${Boost_LIBRARIES})
