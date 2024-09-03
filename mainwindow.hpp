#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include <QtWidgets/QMainWindow>
#include <QtCore/QDir>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Ui::MainWindow ui;

    QDir m_recDir;

    void openRecDir();
    void showRecDirMessage();
    void showRecDurationMessage(qint64 duration);
};
#endif // MAINWINDOW_HPP
