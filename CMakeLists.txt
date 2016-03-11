#  Copyright (c) 2015-2016, Jesper Hellesø Hansen
#  jesperhh@gmail.com
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of the <organization> nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.0)

project(qcloneitem)

set(CMAKE_AUTOMOC true)
set(CMAKE_INCLUDE_CURRENT_DIR true)

option(include_example "Set up example in build dir" OFF)

find_package(Qt5 COMPONENTS Quick REQUIRED)

add_library(qcloneitem SHARED
    qcloneitemplugin.cpp
    qcloneitem.cpp
    qcloneitem_p.h
    qcloneitem.h)

target_compile_options(qcloneitem PRIVATE -W4 -wd4127 -wd6326)
target_compile_definitions(qcloneitem PRIVATE _UNICODE UNICODE WIN32_LEAN_AND_MEAN NOMINMAX VC_EXTRALEAN)
target_link_libraries(qcloneitem Qt5::Quick)

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

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_FILE_NAME	"qcloneitem")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
include(CPack)

install(TARGETS qcloneitem RUNTIME DESTINATION clone)
install(FILES qmldir DESTINATION clone)
install(TARGETS cloneexample DESTINATION example)
install(FILES example/main.qml DESTINATION example)
install(FILES README.md DESTINATION .)
