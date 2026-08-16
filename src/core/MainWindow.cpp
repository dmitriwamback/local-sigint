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

    mainLayout = new QHBoxLayout();

    centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);

    processTable = new QTableWidget();
    processTable->setColumnCount(2);
    processTable->setHorizontalHeaderLabels({"PID", "Process"});
    processTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    packetTable = new QTableWidget();
    packetTable->setColumnCount(2);
    packetTable->setHorizontalHeaderLabels({"DNS Hostname", "Occurences"});
    packetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    packetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    keystrokesTable = new QTableWidget();
    keystrokesTable->setColumnCount(2);
    keystrokesTable->setHorizontalHeaderLabels({"Keystrokes", "Timestamp"});
    keystrokesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    keystrokesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(processTable);
    mainLayout->addWidget(packetTable);
    mainLayout->addWidget(keystrokesTable);
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.show();

    SearchProcesses();
}

void MainWindow::SearchProcesses() {

    std::vector<Process> processes;

#if defined(__APPLE__)
    processes = NSProcessResolver::GetProcesses();
    for (Process process : processes) {
        std::cout << process.name << std::endl;
        int row = processTable->rowCount();
        processTable->insertRow(row);
        processTable->setItem(row, 0, new QTableWidgetItem(QString::number(process.pid)));
        processTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(process.name)));
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
