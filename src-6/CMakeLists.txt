cmake_minimum_required(VERSION 3.10)
project(GlobalQSSPlugin)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(globalqssplugin SHARED
    globalqssplugin.cpp
    globalqss.cpp
    globalqssplugin.h
    globalqss.h
)

set_source_files_properties(globalqssplugin.json PROPERTIES QT_RESOURCE_ALIAS "globalqssplugin.json")

target_link_libraries(globalqssplugin Qt6::Widgets)
target_compile_definitions(globalqssplugin PRIVATE QT_PLUGIN)
