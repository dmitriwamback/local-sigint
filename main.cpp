#include <iostream>

#include <QApplication>

#include "src/keystrokes/Keystrokes.h"
#include "src/keystrokes/Clipboard.h"
#include "src/packets/HostResolver.h"

#if defined(__APPLE__)
#include "src/process/macos/NSProcessResolver.h"
#elif defined(_WIN32)
#include "src/process/windows/Win32ProcessResolver.h"
#endif

#include "src/core/MainWindow.h"

int main() {
    std::cout << "Hello World!" << std::endl;
}