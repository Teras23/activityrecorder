cmake_minimum_required(VERSION 3.12)
project(ActivityTracker)

set(CMAKE_CXX_FLAGS "-luser32 -lkernel32 -lpsapi")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

qt5_wrap_cpp(UICPP
        src/ui/mainwindow.h
        src/ui/mainwindow.h)

qt5_wrap_ui(UIS
        src/ui/mainwindow.ui
        src/ui/infowindow.ui)

qt5_add_resources(RES
        resources.qrc)

set(SOURCES
        src/main.cpp
        src/ui/mainwindow.cpp
        src/tracking/process.cpp
        src/tracking/file.cpp
        src/tracking/entry.cpp
        src/ui/infowindow.cpp
        src/tracking/filedata.cpp
        src/tracking/processinfo.cpp
        src/ui/statistics.cpp)

add_executable(ActivityTracker ${SOURCES} ${UICPP} ${UIS} ${RES})
qt5_use_modules(ActivityTracker Core Widgets Gui)

target_link_libraries(ActivityTracker psapi)
target_link_libraries(ActivityTracker kernel32)
target_link_libraries(ActivityTracker user32)

target_link_libraries(ActivityTracker Qt5::Core)
target_link_libraries(ActivityTracker Qt5::Widgets)
target_link_libraries(ActivityTracker Qt5::Gui)