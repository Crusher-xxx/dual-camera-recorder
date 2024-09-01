#include "camera.hpp"

Camera::Camera(QWidget *parent)
    : QWidget{parent}
{
    ui.setupUi(this);

    m_mediaCaptureSession.setVideoOutput(&m_graphicsVideoItem);
    m_mediaCaptureSession.setCamera(&m_camera);

    ui.graphicsView->setScene(&m_graphicsScene);
    m_graphicsScene.addItem(&m_graphicsVideoItem);
}

void Camera::setCameraDevice(const QCameraDevice &cameraDevice)
{
    m_camera.setCameraDevice(cameraDevice);
    m_camera.start();
    ui.label->setText(m_camera.cameraDevice().description());
}
