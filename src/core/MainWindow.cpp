//
// Created by Dmitri on 2026-08-16.
//

#include "MainWindow.h"

#include <iostream>
#include <ostream>

#if defined(__APPLE__)
#include "../process/macos/NSProcessResolver.h"
#elif defined(_WIN32)
#include "../process/windows/Win32ProcessResolver.h"
#endif

MainWindow::MainWindow() {
    mainWindow.setWindowTitle("SIGINT");
    mainWindow.resize(1200, 800);
    mainWindow.show();

    SearchProcesses();
}

void MainWindow::SearchProcesses() {

    std::vector<Process> processes;

#if defined(__APPLE__)
    processes = NSProcessResolver::GetProcesses();
    for (Process process : processes) {
        std::cout << process.name << std::endl;
    }
#elif defined(_WIN32)

#endif

}

void MainWindow::DecodePackets() {

}

void MainWindow::RecordKeystrokes() {
    if (!recordKeystrokes) return;
}

void MainWindow::RecordClipboard() {
    if (!recordClipboard) return;
}
