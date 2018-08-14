cmake_minimum_required(VERSION 3.12)
project(ActivityTracker)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

qt5_wrap_cpp(UICPP
        src/ui/mainwindow.h
        src/ui/mainwindow.h)

qt5_wrap_ui(UIS
        src/ui/mainwindow.ui
        src/ui/infowindow.ui)

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

add_executable(ActivityTracker ${SOURCES} ${UIS} ${UICPP})
target_link_libraries(ActivityTracker Qt5::Widgets Qt5::Core)