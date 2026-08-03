cmake_minimum_required(VERSION 3.16)

project(Common LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(Common SHARED
  Common_global.h
  common.cpp
  common.h

  media.h
  media.cpp
  CommonFunctions.cpp
)

target_link_libraries(Common PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(Common PRIVATE COMMON_LIBRARY)
