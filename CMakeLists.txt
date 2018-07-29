cmake_minimum_required(VERSION 3.12)
project(QtTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(SOURCES
        src/main.cpp
        src/ui/mainwindow.cpp
        src/tracking/process.cpp
        src/tracking/file.cpp
        src/tracking/entry.cpp
        src/ui/infowindow.cpp
        src/tracking/filedata.cpp
        src/tracking/processinfo.cpp
        src/ui/statistics.cpp
        src/ui/mainwindow.cpp
        resources.qrc)

add_executable(QtTest ${SOURCES})
target_link_libraries(QtTest Qt5::Widgets Qt5::Core)