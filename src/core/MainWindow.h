//
// Created by Dmitri on 2026-08-16.
//

#ifndef SIGINT_MAINWINDOW_H
#define SIGINT_MAINWINDOW_H

#include <QMainWindow>

class MainWindow {
public:
    MainWindow();
    void SearchProcesses();
    void DecodePackets();
    void RecordKeystrokes();
    void RecordClipboard();
private:
    bool recordKeystrokes = false;
    bool recordClipboard = false;

    QMainWindow mainWindow;
};


#endif //SIGINT_MAINWINDOW_H
