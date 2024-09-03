#include "mainwindow.hpp"

#include <QtMultimedia/QMediaDevices>
#include <QtGui/QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_recDir{QDir::current().path() + "/rec"}
{
    ui.setupUi(this);

    auto videoDevices{QMediaDevices::videoInputs()};
    ui.cam1->setCameraDevice(videoDevices[0]);
    ui.cam2->setCameraDevice(videoDevices[1]);

    m_recDir.mkpath(".");
    ui.cam1->m_recDir = m_recDir;
    ui.cam2->m_recDir = m_recDir;

    ui.cam1->m_cameraIdentifier = "cam1";
    ui.cam2->m_cameraIdentifier = "cam2";

    connect(ui.actionShowCrosshair, &QAction::toggled, ui.cam1, &Camera::setCrosshairVisible);
    connect(ui.actionShowCrosshair, &QAction::toggled, ui.cam2, &Camera::setCrosshairVisible);
    connect(ui.actionTakeSnapshot, &QAction::triggered, ui.cam1, &Camera::saveImage);
    connect(ui.actionTakeSnapshot, &QAction::triggered, ui.cam2, &Camera::saveImage);
    connect(ui.actionOpenRecordings, &QAction::triggered, this, &MainWindow::openRecDir);
    connect(ui.cam2, &Camera::imageSaved, this, &MainWindow::showRecDirMessage);
}

void MainWindow::showRecDirMessage()
{
    ui.statusbar->showMessage("Image saved: " + m_recDir.path());
}

void MainWindow::openRecDir()
{
    QDesktopServices::openUrl(m_recDir.absolutePath());
}
