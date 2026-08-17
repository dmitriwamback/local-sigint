//
// Created by Dmitri on 2026-08-16.
//

#include "MainWindow.h"

#include <iostream>
#include <ostream>

#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>

#if defined(__APPLE__)
#include "../process/macos/NSProcessResolver.h"
#elif defined(_WIN32)
#include "../process/windows/Win32ProcessResolver.h"
#endif


MainWindow::MainWindow(): QObject(nullptr) {

    mainWindow.setWindowTitle("SIGINT");
    mainWindow.resize(1200, 600);

    mainLayout = new QHBoxLayout();

    centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);

    processTable = new QTableWidget();
    processTable->setColumnCount(2);
    processTable->setHorizontalHeaderLabels({"PID", "Process"});
    processTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTable->setSelectionMode(QAbstractItemView::SingleSelection);
    processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);


    packetTable = new QTableWidget();
    packetTable->setColumnCount(2);
    packetTable->setHorizontalHeaderLabels({"DNS Hostname", "Count"});
    packetTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    packetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    packetTable->setSelectionMode(QAbstractItemView::SingleSelection);
    packetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);


    keystrokesTable = new QTableWidget();
    keystrokesTable->setColumnCount(2);
    keystrokesTable->setHorizontalHeaderLabels({"Keystrokes", "Timestamp"});
    keystrokesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    keystrokesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    keystrokesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    keystrokesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    processTable->installEventFilter(this);
    packetTable->installEventFilter(this);
    keystrokesTable->installEventFilter(this);

    mainLayout->addWidget(processTable);
    mainLayout->addWidget(packetTable);
    mainLayout->addWidget(keystrokesTable);
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.show();

    ResizeTables();
    SearchProcesses();
}

void MainWindow::ResizeTables() {

    int processWidth = processTable->viewport()->width();
    processTable->setColumnWidth(0, processWidth * 0.20);
    processTable->setColumnWidth(1, processWidth * 0.80);


    int packetWidth = packetTable->viewport()->width();
    packetTable->setColumnWidth(0, packetWidth * 0.80);
    packetTable->setColumnWidth(1, packetWidth * 0.20);


    int keystrokeWidth = keystrokesTable->viewport()->width();
    keystrokesTable->setColumnWidth(0, keystrokeWidth * 0.60);
    keystrokesTable->setColumnWidth(1, keystrokeWidth * 0.40);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Resize) {

        if (watched == processTable || watched == packetTable || watched == keystrokesTable) {
            ResizeTables();
        }
    }

    return QObject::eventFilter(watched, event);
}


void MainWindow::SearchProcesses() {

    processTable->setRowCount(0);
    std::vector<Process> processes;


#if defined(__APPLE__)
    processes = NSProcessResolver::GetProcesses();
#elif defined(_WIN32)

#endif


    for (const Process& process : processes) {

        int row = processTable->rowCount();

        processTable->insertRow(row);
        processTable->setItem(row, 0, new QTableWidgetItem(QString::number(process.pid)));
        processTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(process.name)));
    }
}

void MainWindow::DecodePackets() {

}

void MainWindow::RecordKeystrokes() {
    if (!recordKeystrokesEnabled) return;
}

void MainWindow::RecordClipboard() {
    if (!recordClipboardEnabled) return;
}