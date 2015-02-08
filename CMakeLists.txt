cmake_minimum_required(VERSION 3.0)

project(qcloneitem)

set(CMAKE_PREFIX_PATH "C:/Qt/5.4/msvc2013_64" CACHE PATH "Qt 5 install dir")
set(CMAKE_AUTOMOC true)
set(CMAKE_INCLUDE_CURRENT_DIR true)

option(include_example "Set up example in build dir" OFF)

find_package(Qt5 COMPONENTS Quick REQUIRED)

add_library(qcloneitem SHARED
    qcloneitemplugin.cpp
    qcloneitem.cpp
    qcloneitem.h)

target_compile_options(qcloneitem PRIVATE -W4 -wd4127 -wd6326 -analyze)
target_compile_definitions(qcloneitem PRIVATE _UNICODE UNICODE)
target_link_libraries(qcloneitem Qt5::Quick)

if(include_example)
    add_custom_target(output_dir_copy ALL DEPENDS
        ${CMAKE_BINARY_DIR}/run.cmd
        ${CMAKE_BINARY_DIR}/main.qml
        ${CMAKE_BINARY_DIR}/Clone/qmldir
        ${CMAKE_BINARY_DIR}/Clone/qcloneitem.dll)

    add_custom_command(OUTPUT
        ${CMAKE_BINARY_DIR}/run.cmd
        ${CMAKE_BINARY_DIR}/main.qml
        ${CMAKE_BINARY_DIR}/Clone/qmldir
        ${CMAKE_BINARY_DIR}/Clone/qcloneitem.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/example/run.cmd" "${CMAKE_BINARY_DIR}/run.cmd"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/example/main.qml" "${CMAKE_BINARY_DIR}/main.qml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/qmldir" "${CMAKE_BINARY_DIR}/Clone/qmldir"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:qcloneitem>" "${CMAKE_BINARY_DIR}/Clone/qcloneitem.dll"
        DEPENDS ${CMAKE_SOURCE_DIR}/main.qml ${CMAKE_SOURCE_DIR}/qmldir qcloneitem)
endif(include_example)