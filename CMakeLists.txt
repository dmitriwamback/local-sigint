cmake_minimum_required(VERSION 4.3)
project(sigint)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
        Widgets
        OpenGL
        OpenGLWidgets
)

# ============================================================
# libpcap
# ============================================================

if(APPLE)

    find_path(PCAP_INCLUDE_DIR
            NAMES pcap.h
            PATHS
            /opt/homebrew/opt/libpcap/include
            /usr/local/opt/libpcap/include
            REQUIRED
    )

    find_library(PCAP_LIBRARY
            NAMES pcap
            PATHS
            /opt/homebrew/opt/libpcap/lib
            /usr/local/opt/libpcap/lib
            REQUIRED
    )

elseif(WIN32)

    find_path(PCAP_INCLUDE_DIR
            NAMES pcap.h
            PATHS
            "C:/Program Files/Npcap/Include"
            REQUIRED
    )

    find_library(PCAP_LIBRARY
            NAMES wpcap
            PATHS
            "C:/Program Files/Npcap"
            REQUIRED
    )

endif()

# ============================================================
# Executable
# ============================================================

qt_add_executable(sigint
        main.cpp

        src/keystrokes/Keystrokes.cpp
        src/keystrokes/Keystrokes.h

        src/keystrokes/Clipboard.cpp
        src/keystrokes/Clipboard.h

        src/packets/DNSCapture.cpp
        src/packets/DNSCapture.h

        src/process/macos/NSProcessResolver.cpp
        src/process/macos/NSProcessResolver.h

        src/process/windows/Win32ProcessResolver.cpp
        src/process/windows/Win32ProcessResolver.h

        src/core/MainWindow.cpp
        src/core/MainWindow.h

        src/process/Process.h
)

# ============================================================
# Include directories
# ============================================================

target_include_directories(sigint PRIVATE
        ${PCAP_INCLUDE_DIR}
)

# ============================================================
# Link libraries
# ============================================================

target_link_libraries(sigint PRIVATE
        Qt6::Widgets
        ${PCAP_LIBRARY}
)