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
    Q_PROPERTY(bool crosshairVisible READ crosshairVisible WRITE setCrosshairVisible NOTIFY crosshairVisibleChanged)

signals:
    void crosshairVisibleChanged(bool visible);

public:
    explicit Camera(QWidget *parent = nullptr);
    void setCameraDevice(const QCameraDevice &cameraDevice);

    bool crosshairVisible();
    void setCrosshairVisible(bool visible);

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
    bool m_crosshairVisible;

    void repositionScene();
    void resizeEvent(QResizeEvent *event) override;
};

#endif // CAMERA_HPP
