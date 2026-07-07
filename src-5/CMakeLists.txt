# build
cmake_minimum_required(VERSION 3.10)
project(GlobalQSSPlugin)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
if(UNIX)
	find_package(Qt5 COMPONENTS DBus REQUIRED)
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
target_link_libraries(globalqssplugin Qt5::Widgets)
if(UNIX)
	target_link_libraries(globalqssplugin Qt5::DBus)
endif()

target_compile_definitions(globalqssplugin PRIVATE QT_PLUGIN)

# install
get_target_property(QT5_QMAKE Qt5::qmake IMPORTED_LOCATION)
execute_process(
	COMMAND ${QT5_QMAKE} -query QT_INSTALL_PLUGINS
	OUTPUT_VARIABLE QT5_PLUGINS_DIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Qt5 plugins dir: ${QT5_PLUGINS_DIR}")

include(GNUInstallDirs)
install(TARGETS globalqssplugin
	LIBRARY DESTINATION ${QT5_PLUGINS_DIR}/styles
)
