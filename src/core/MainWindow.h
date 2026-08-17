//
// Created by Dmitri on 2026-08-16.
//

#ifndef SIGINT_MAINWINDOW_H
#define SIGINT_MAINWINDOW_H


#include <QMainWindow>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableWidget>
#include <QEvent>

class MainWindow : public QObject {
public:
    MainWindow();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QMainWindow mainWindow;

    QHBoxLayout* mainLayout;
    QWidget* centralWidget;

    QTableWidget* processTable;
    QTableWidget* packetTable;
    QTableWidget* keystrokesTable;

    bool recordKeystrokesEnabled = false;
    bool recordClipboardEnabled = false;

    void SearchProcesses();
    void DecodePackets();
    void RecordKeystrokes();
    void RecordClipboard();

    void ResizeTables();
};


#endif //SIGINT_MAINWINDOW_H
