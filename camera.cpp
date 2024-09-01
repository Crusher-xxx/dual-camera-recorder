#include "camera.hpp"

Camera::Camera(QWidget *parent)
    : QWidget{parent}
{
    ui.setupUi(this);

    m_mediaCaptureSession.setVideoOutput(&m_graphicsVideoItem);
    m_mediaCaptureSession.setCamera(&m_camera);

    ui.graphicsView->setScene(&m_graphicsScene);
    m_graphicsScene.addItem(&m_graphicsVideoItem);

    connect(&m_graphicsVideoItem, &QGraphicsVideoItem::nativeSizeChanged, this, &Camera::repositionScene);
}

void Camera::setCameraDevice(const QCameraDevice &cameraDevice)
{
    m_camera.setCameraDevice(cameraDevice);
    m_camera.start();
    ui.label->setText(m_camera.cameraDevice().description());
}

void Camera::repositionScene()
{
    ui.graphicsView->fitInView(&m_graphicsVideoItem, Qt::KeepAspectRatio);
}

void Camera::resizeEvent(QResizeEvent *event)
{
    // Zoom to video
    ui.graphicsView->fitInView(&m_graphicsVideoItem, Qt::KeepAspectRatio);
}
