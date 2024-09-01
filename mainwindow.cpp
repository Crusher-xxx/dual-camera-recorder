#include "mainwindow.hpp"

#include <QtMultimedia/QMediaDevices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    ui.setupUi(this);

    auto videoDevices{QMediaDevices::videoInputs()};
    ui.cam1->setCameraDevice(videoDevices[0]);
    ui.cam2->setCameraDevice(videoDevices[1]);
}
