cmake_minimum_required(VERSION 3.10)

project(Indexer)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Xml)

include_directories(${PROJECT_SOURCE_DIR}/Indexer ${PROJECT_SOURCE_DIR}/XMLParser)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Qt6::Core Qt6::Xml)

add_custom_target(run COMMAND main)
