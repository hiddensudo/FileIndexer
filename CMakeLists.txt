cmake_minimum_required(VERSION 3.10)

project(FileIndexer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Xml Quick)

include_directories(${PROJECT_SOURCE_DIR}/DirectoryManagment/Indexer
                    ${PROJECT_SOURCE_DIR}/DirectoryManagment/PathQueue
                    ${PROJECT_SOURCE_DIR}/MyApp
                    ${PROJECT_SOURCE_DIR}/XML/XMLParser
                    ${PROJECT_SOURCE_DIR}/XML/XMLWriter
                    ${PROJECT_SOURCE_DIR}/XML/XMLReader)

add_executable(main 
    main.cpp
    MyApp/MyApp.cpp
    XML/XMLParser/XMLParser.cpp
    XML/XMLWriter/XMLWriter.cpp
    DirectoryManagment/PathQueue/PathQueue.cpp
    DirectoryManagment/Indexer/Indexer.cpp
    qml.qrc
)

target_link_libraries(main PRIVATE Qt6::Core Qt6::Xml Qt6::Quick)

add_custom_target(run COMMAND main)
