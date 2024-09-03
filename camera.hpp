#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ui_camera.h"

#include <QtCore/QDir>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtMultimedia/QMediaCaptureSession>
#include <QtMultimedia/QCamera>
#include <QtMultimediaWidgets/QGraphicsVideoItem>
#include <QtMultimedia/QMediaRecorder>
#include <QtMultimedia/QImageCapture>

class Camera : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool crosshairVisible READ crosshairVisible WRITE setCrosshairVisible NOTIFY crosshairVisibleChanged)

signals:
    void crosshairVisibleChanged(bool visible);
    void imageSaved();

public:
    explicit Camera(QWidget *parent = nullptr);
    void setCameraDevice(const QCameraDevice &cameraDevice);
    void saveImage();

    bool crosshairVisible();
    void setCrosshairVisible(bool visible);

    QDir m_recDir;
    QString m_cameraIdentifier;

private:
    Ui::Camera ui;

    QMediaCaptureSession m_mediaCaptureSession;
    QCamera m_camera;

    // Camera output display
    QGraphicsScene m_graphicsScene;
    QGraphicsVideoItem m_graphicsVideoItem;

    // Crosshair
    QGraphicsLineItem m_hLine;
    QGraphicsLineItem m_vLine;
    QGraphicsPixmapItem m_crosshair;
    qreal m_crosshairScale;
    bool m_crosshairVisible;

    // Camera output
    QImageCapture m_imageCapture;

    void repositionScene();
    void resizeEvent(QResizeEvent *event) override;
    QString generateFilePath();
};

#endif // CAMERA_HPP
