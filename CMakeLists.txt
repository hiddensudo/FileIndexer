cmake_minimum_required(VERSION 3.16)

project(FileIndexer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Xml Quick)

include_directories(${PROJECT_SOURCE_DIR}/Indexer
                    ${PROJECT_SOURCE_DIR}/MyApp
                    ${PROJECT_SOURCE_DIR}/XML/XMLWriter
                    ${PROJECT_SOURCE_DIR}/XML/XMLParser
                    ${PROJECT_SOURCE_DIR}/FillModel)

add_executable(main 
    main.cpp
    MyApp/MyApp.cpp
    XML/XMLWriter/XMLWriter.cpp
    XML/XMLParser/XMLParser.cpp
    FillModel/FillModel.cpp
    FillModel/CustomStandardItemModel.cpp
    Indexer/Indexer.cpp
    qml.qrc
)

target_link_libraries(main PRIVATE Qt6::Core Qt6::Xml Qt6::Quick)

add_custom_target(run COMMAND main)
