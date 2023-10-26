cmake_minimum_required(VERSION 3.26)
project(RE_qt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH "D:/qt5/data/5.14.2/mingw73_64")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(.)

if (ANDROID)
    add_executable(RE_qt
            WIN32
            main.cpp
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
            NFAmodel.h
            NFAmodel.cpp
            DFAmodel.h
            DFAmodel.cpp
            ctr_file.h
            ctr_file.cpp
            REmodel.h
            REmodel.cpp)
else ()
    add_executable(RE_qt
            WIN32
            main.cpp
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
            NFAmodel.h
            NFAmodel.cpp
            DFAmodel.h
            DFAmodel.cpp
            ctr_file.h
            ctr_file.cpp
            REmodel.h
            REmodel.cpp)
endif ()

target_link_libraries(RE_qt PRIVATE Qt5::Widgets)