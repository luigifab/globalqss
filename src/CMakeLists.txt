# build
cmake_minimum_required(VERSION 3.10)
project(GlobalQSSPlugin)
set(QT_MAJOR 6 CACHE STRING "Qt major version (5 or 6)")

find_package(Qt${QT_MAJOR} COMPONENTS Widgets REQUIRED)
if(UNIX)
	find_package(Qt${QT_MAJOR} COMPONENTS DBus REQUIRED)
endif()

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
target_link_libraries(globalqssplugin Qt${QT_MAJOR}::Widgets)
if(UNIX)
	target_link_libraries(globalqssplugin Qt${QT_MAJOR}::DBus)
endif()

target_compile_definitions(globalqssplugin PRIVATE QT_PLUGIN)

# install
get_target_property(QT_QMAKE Qt${QT_MAJOR}::qmake IMPORTED_LOCATION)
execute_process(
	COMMAND ${QT_QMAKE} -query QT_INSTALL_PLUGINS
	OUTPUT_VARIABLE QT_PLUGINS_DIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Qt${QT_MAJOR} plugins dir: ${QT_PLUGINS_DIR}")

install(TARGETS globalqssplugin
	LIBRARY DESTINATION ${QT_PLUGINS_DIR}/styles
)
