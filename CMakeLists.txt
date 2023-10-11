cmake_minimum_required(VERSION 3.10)

project(Indexer)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/Indexer)

add_executable(main main.cpp)

add_custom_target(run COMMAND main)