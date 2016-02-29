cmake_minimum_required(VERSION 3.0)

project(qcloneitem)

set(CMAKE_AUTOMOC true)
set(CMAKE_INCLUDE_CURRENT_DIR true)

set(CMAKE_PREFIX_PATH "D:/Qt/5.5/msvc2013_64/")
option(include_example "Set up example in build dir" OFF)

find_package(Qt5 COMPONENTS Quick REQUIRED)

add_library(qcloneitem SHARED
    qcloneitemplugin.cpp
    qcloneitem.cpp
    qcloneitem_p.h
    qcloneitem.h)

target_compile_options(qcloneitem PRIVATE -W4 -wd4127 -wd6326)
target_compile_definitions(qcloneitem PRIVATE _UNICODE UNICODE)
target_link_libraries(qcloneitem Qt5::Quick)

if(include_example)
    set(example_dir ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
    add_executable(cloneexample WIN32
        example/main.cpp
        ${example_dir}/Clone/qmldir
        ${example_dir}/Clone/qcloneitem.dll
        ${example_dir}/main.qml)

    target_link_libraries(cloneexample Qt5::Quick)

    add_custom_command(OUTPUT
        ${example_dir}/main.qml
        ${example_dir}/Clone/qmldir
        ${example_dir}/Clone/qcloneitem.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/example/main.qml" "${example_dir}/main.qml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/qmldir" "${example_dir}/Clone/qmldir"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:qcloneitem>" "${example_dir}/Clone/qcloneitem.dll"
        DEPENDS ${CMAKE_SOURCE_DIR}/main.qml ${CMAKE_SOURCE_DIR}/qmldir qcloneitem
        COMMENT "Copying to output directory")
endif(include_example)