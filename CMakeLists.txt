cmake_minimum_required(VERSION 3.0)

project(qclonewindow)

set(CMAKE_PREFIX_PATH "C:/Qt/5.4/msvc2013_64" CACHE PATH "Qt 5 install dir")
set(CMAKE_AUTOMOC true)

find_package(Qt5 COMPONENTS Qml Gui Quick Widgets REQUIRED)

add_definitions(-D_UNICODE -DUNICODE)

add_executable(qclonewindow #WIN32
    main.cpp 
    qcloneitem.cpp 
    qcloneitem.h
    main.qml)

add_dependencies(qclonewindow output_dir_copy)

target_compile_options(qclonewindow PRIVATE -W4 -wd4127)
target_link_libraries(qclonewindow Qt5::Qml Qt5::Gui Qt5::Quick Qt5::Widgets)

add_custom_target(output_dir_copy DEPENDS ${CMAKE_BINARY_DIR}/main.qml)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/main.qml
					COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/main.qml" "${CMAKE_BINARY_DIR}/main.qml"
					DEPENDS ${CMAKE_SOURCE_DIR}/main.qml)