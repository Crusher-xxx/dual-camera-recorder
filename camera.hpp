#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ui_camera.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtMultimedia/QMediaCaptureSession>
#include <QtMultimedia/QCamera>
#include <QtMultimediaWidgets/QGraphicsVideoItem>

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    void setCameraDevice(const QCameraDevice &cameraDevice);

private:
    Ui::Camera ui;

    QGraphicsScene m_graphicsScene;  // Offscreen container
    QMediaCaptureSession m_mediaCaptureSession;
    QCamera m_camera;
    QGraphicsVideoItem m_graphicsVideoItem;

    QGraphicsLineItem m_hLine;
    QGraphicsLineItem m_vLine;
    QGraphicsPixmapItem m_crosshair;
    qreal m_crosshairScale;

    void repositionScene();
    void resizeEvent(QResizeEvent *event) override;
};

#endif // CAMERA_HPP
