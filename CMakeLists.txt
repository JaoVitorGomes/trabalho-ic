cmake_minimum_required(VERSION 3.14)
project(trabalho-ic CXX)

set(INPUT_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/header/config.h) 

include_directories(header)
file(GLOB source_files src/*.cpp)
add_library(ic ${source_files})
target_include_directories(ic PUBLIC ${PROJECT_SOURCE_DIR}/header)

add_subdirectory(tests)
enable_testing()
add_test(NAME test1 COMMAND test1)

add_executable(main app/main.cpp)
target_link_libraries(main ic)




