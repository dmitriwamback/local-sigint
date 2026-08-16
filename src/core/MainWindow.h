//
// Created by Dmitri on 2026-08-16.
//

#ifndef SIGINT_MAINWINDOW_H
#define SIGINT_MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

class MainWindow {
public:
    MainWindow();
    void SearchProcesses();
    void DecodePackets();
    void RecordKeystrokes();
    void RecordClipboard();
private:
    bool recordKeystrokesEnabled = false;
    bool recordClipboardEnabled = false;

    QMainWindow mainWindow;

    QWidget* centralWidget;
    QHBoxLayout* mainLayout;
    QTableWidget *processTable, *packetTable, *keystrokesTable;
};


#endif //SIGINT_MAINWINDOW_H
