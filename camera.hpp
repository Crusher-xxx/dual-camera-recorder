#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ui_camera.h"

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);

private:
    Ui::Camera ui;
};

#endif // CAMERA_HPP
