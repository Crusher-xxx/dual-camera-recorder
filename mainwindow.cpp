#include "mainwindow.hpp"

#include <QtMultimedia/QMediaDevices>
#include <QtGui/QDesktopServices>
#include <QKeyEvent>
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_recDir{QDir::current().path() + "/rec"}
{
    ui.setupUi(this);

    QList<QCameraDevice> videoDevices{QMediaDevices::videoInputs()};
    while (videoDevices.length() < 2) {
        QString msg{"App requires 2 cameras. Found %1. Connect more cameras and click `Retry`"};
        msg = msg.arg(videoDevices.length());

        QMessageBox msgBox{};
        msgBox.setText(msg);
        msgBox.setStandardButtons(QMessageBox::Button::Retry | QMessageBox::Button::Abort);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Button::Abort)
        {
            throw std::runtime_error(msgBox.text().toStdString());
        }

        videoDevices = QMediaDevices::videoInputs();
    }
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
    connect(ui.cam2, &Camera::recDurationChaged, this, &MainWindow::showRecDurationMessage);

    connect(ui.cam2, &Camera::recStateChanged, this, [this](QMediaRecorder::RecorderState state){ui.pushButtonStartRecording->setDisabled(state == QMediaRecorder::RecorderState::RecordingState);});
    connect(ui.cam2, &Camera::recStateChanged, this, [this](QMediaRecorder::RecorderState state){ui.pushButtonStopRecording->setEnabled(state != QMediaRecorder::RecorderState::StoppedState);});
    connect(ui.pushButtonStartRecording, &QPushButton::clicked, ui.cam1, &Camera::startRecording);
    connect(ui.pushButtonStartRecording, &QPushButton::clicked, ui.cam2, &Camera::startRecording);
    connect(ui.pushButtonStopRecording, &QPushButton::clicked, ui.cam1, &Camera::stopRecording);
    connect(ui.pushButtonStopRecording, &QPushButton::clicked, ui.cam2, &Camera::stopRecording);
}

void MainWindow::openRecDir()
{
    QDesktopServices::openUrl(m_recDir.absolutePath());
}

void MainWindow::showRecDirMessage()
{
    ui.statusbar->showMessage("Images saved to directory: " + m_recDir.path());
}

void MainWindow::showRecDurationMessage(qint64 duration)
{
    auto milliseconds{duration};
    auto seconds{milliseconds / 1000};
    auto minutes{seconds / 60};
    auto hours{minutes / 60};

    milliseconds %= 1000;
    seconds %= 60;
    minutes %= 60;

    QString msg{"Duration: %1:%2:%3.%4"};
    QChar fillChar{'0'};
    msg = msg
              .arg(hours, 2, 10, fillChar)
              .arg(minutes, 2, 10, fillChar)
              .arg(seconds, 2, 10, fillChar)
              .arg(milliseconds, 2, 10, fillChar);

    ui.statusbar->showMessage(msg);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key::Key_Enter:
    case Qt::Key::Key_Return:
        ui.cam1->saveImage();
        ui.cam2->saveImage();
        break;
    default:
        break;
    }
}
