#include <iostream>
#include <string>

#include <QApplication>

#include "src/keystrokes/Keystrokes.h"
#include "src/keystrokes/Clipboard.h"
#include "src/packets/DNSCapture.h"

#include "src/process/Process.h"

#if defined(__APPLE__)
#include "src/process/macos/NSProcessResolver.h"
#elif defined(_WIN32)
#include "src/process/windows/Win32ProcessResolver.h"
#endif

#include "src/core/MainWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWindow mainWindow = MainWindow();

    return QApplication::exec();
}