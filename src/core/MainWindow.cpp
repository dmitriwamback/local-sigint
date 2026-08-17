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

    mainWindow.setStyleSheet(R"(
        QMainWindow {
            background: #0d1117;
        }

        QWidget {
            background: #0d1117;
            color: #c9d1d9;
            font-family: "SF Pro Display";
            font-size: 13px;
        }

        QTableWidget {
            background: #11161d;
            alternate-background-color: #0f141a;
            border: 1px solid #30363d;
            gridline-color: #21262d;
            selection-background-color: #1f6feb;
            selection-color: white;
        }

        QHeaderView::section {
            background: #161b22;
            color: #8b949e;
            border: none;
            border-bottom: 1px solid #30363d;
            padding: 8px;
            font-weight: bold;
        }

        QTableWidget::item {
            padding: 6px;
        }

        QScrollBar:vertical {
            background: #0d1117;
            width: 8px;
        }

        QScrollBar::handle:vertical {
            background: #30363d;
            border-radius: 4px;
        }
    )");

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

    dnsCapture = new DNSCapture();

    dnsCapture->Start([this](const std::string& host) {
       QMetaObject::invokeMethod(this, [this, host]() {
           OnDNSQuery(host);
       },
       Qt::QueuedConnection);
    });
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

void MainWindow::OnDNSQuery(const std::string& host) {
    QString hostname = QString::fromStdString(host);

    for (int row = 0; row < packetTable->rowCount(); row++) {
        if (packetTable->item(row, 0)->text() == hostname) {
            int count = packetTable->item(row, 1)->text().toInt();
            packetTable->item(row, 1)->setText(QString::number(count+1));
            return;
        }
    }

    int row = packetTable->rowCount();
    packetTable->insertRow(row);

    packetTable->setItem(row, 0, new QTableWidgetItem(hostname));
    packetTable->setItem(row, 1, new QTableWidgetItem("1"));
}

void MainWindow::RecordKeystrokes() {
    if (!recordKeystrokesEnabled) return;
}

void MainWindow::RecordClipboard() {
    if (!recordClipboardEnabled) return;
}