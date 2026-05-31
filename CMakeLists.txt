cmake_minimum_required(VERSION 3.20)
project(LWZip VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)

# Platform detection
if(APPLE)
    set(LWZIP_PLATFORM "macOS")
    add_definitions(-D__APPLE__)
elseif(WIN32)
    set(LWZIP_PLATFORM "Windows")
    add_definitions(-D_WIN32 -DUNICODE -D_UNICODE)
endif()

message(STATUS "Building LWZip for ${LWZIP_PLATFORM}")

# 7zip core sources
add_subdirectory(GUI)
