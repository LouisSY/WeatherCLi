cmake_minimum_required(VERSION 3.21)
project(WeatherCLi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network)

qt_standard_project_setup()

qt_add_executable(WeatherCLi
    main.cpp
    weatherbackend.cpp
    weatherbackend.h
)

qt_add_resources(WeatherCLi "qml_resources"
    PREFIX "/"
    FILES
        main.qml
)

target_link_libraries(WeatherCLi PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
)

set_target_properties(WeatherCLi PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
