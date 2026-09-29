cmake_minimum_required(VERSION 3.20)

project(SystemMonitor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
)

add_executable(SystemMonitor WIN32
        main.cpp
        SystemInfo.cpp
        SystemInfo.h
        console_testing.cpp
        console_testing.h
        kill_processes.cpp
        kill_processes.h
        get_processes.cpp
        get_processes.h
)

target_link_libraries(SystemMonitor
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")

    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    # platforms plugin
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET SystemMonitor POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:SystemMonitor>/plugins/platforms/")

        add_custom_command(TARGET SystemMonitor POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:SystemMonitor>/plugins/platforms/")
    endif ()

    # Qt DLLs
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET SystemMonitor POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:SystemMonitor>")
    endforeach ()
endif ()