cmake_minimum_required(VERSION 4.3)
project(sigint)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    OpenGL
    OpenGLWidgets
)

qt_add_executable(sigint main.cpp
        src/keystrokes/Keystrokes.cpp
        src/keystrokes/Keystrokes.h
        src/keystrokes/Clipboard.cpp
        src/keystrokes/Clipboard.h
        src/packets/HostResolver.cpp
        src/packets/HostResolver.h
        src/process/macos/NSProcessResolver.cpp
        src/process/macos/NSProcessResolver.h
        src/process/windows/Win32ProcessResolver.cpp
        src/process/windows/Win32ProcessResolver.h
        src/process/macos/Bridging-Header.h
        src/core/MainWindow.cpp
        src/core/MainWindow.h)

target_link_libraries(sigint PRIVATE
    Qt6::Widgets
)