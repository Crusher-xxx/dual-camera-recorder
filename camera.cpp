#include "camera.hpp"

Camera::Camera(QWidget *parent)
    : QWidget{parent}
    , m_hLine{&m_graphicsVideoItem}
    , m_vLine{&m_graphicsVideoItem}
    , m_crosshair{QPixmap{":/res/crosshair.png"}, &m_graphicsVideoItem}
    , m_crosshairScale{0.2}
{
    ui.setupUi(this);

    // Hide the crosshair until the video appears
    setCrosshairVisible(false);
    m_crosshairVisible = true;

    m_imageCapture.setQuality(QImageCapture::VeryHighQuality);
    m_mediaRecorder.setQuality(QMediaRecorder::VeryHighQuality);

    m_mediaCaptureSession.setCamera(&m_camera);
    m_mediaCaptureSession.setVideoOutput(&m_graphicsVideoItem);
    m_mediaCaptureSession.setImageCapture(&m_imageCapture);
    m_mediaCaptureSession.setRecorder(&m_mediaRecorder);

    ui.graphicsView->setScene(&m_graphicsScene);
    m_graphicsScene.addItem(&m_graphicsVideoItem);

    // Setting offset so positioning works relatively to the center
    m_crosshair.setOffset(-m_crosshair.boundingRect().width() / 2, -m_crosshair.boundingRect().height() / 2);

    // Styling
    m_graphicsScene.setBackgroundBrush(Qt::GlobalColor::gray);
    QColor crosshairColor{Qt::GlobalColor::red};
    QPen crosshairPen{crosshairColor, 3.5};
    m_hLine.setPen(crosshairPen);
    m_vLine.setPen(crosshairPen);

    m_hLine.setFlag(QGraphicsItem::ItemIgnoresTransformations);
    m_vLine.setFlag(QGraphicsItem::ItemIgnoresTransformations);
    m_crosshair.setFlag(QGraphicsItem::ItemIgnoresTransformations);

    connect(&m_graphicsVideoItem, &QGraphicsVideoItem::nativeSizeChanged, this, &Camera::onVideoLoaded);
    connect(&m_imageCapture, &QImageCapture::imageSaved, this, &Camera::imageSaved);
    connect(&m_mediaRecorder, &QMediaRecorder::durationChanged, this, &Camera::recDurationChaged);
    connect(&m_mediaRecorder, &QMediaRecorder::recorderStateChanged, this, &Camera::recStateChanged);
    connect(ui.graphicsView, &GraphicsView::scaleChanged, this, &Camera::onScaleChanged);
}

Camera::~Camera()
{
    if (m_mediaRecorder.recorderState() != QMediaRecorder::StoppedState)
    {
        m_mediaRecorder.stop();
    }
}

void Camera::setCameraDevice(const QCameraDevice &cameraDevice)
{
    m_camera.setCameraDevice(cameraDevice);
    m_camera.start();
    ui.label->setText(m_camera.cameraDevice().description());
}

void Camera::saveImage()
{
    QString filePath{generateFilePath()};
    m_imageCapture.captureToFile(filePath + ".jpg");
}

void Camera::startRecording()
{
    QString filePath{generateFilePath()};
    m_mediaRecorder.setOutputLocation(filePath + ".mp4");
    m_mediaRecorder.record();
}

void Camera::stopRecording()
{
    m_mediaRecorder.stop();
}

bool Camera::crosshairVisible()
{
    return m_crosshairVisible;
}

void Camera::setCrosshairVisible(bool visible)
{
    m_crosshairVisible = visible;
    m_hLine.setVisible(visible);
    m_vLine.setVisible(visible);
    m_crosshair.setVisible(visible);
    emit crosshairVisibleChanged(visible);
}

void Camera::onVideoLoaded()
{
    // Set the scene to be the same size as the video
    m_graphicsScene.setSceneRect(m_graphicsVideoItem.boundingRect());

    // Scale proportionally to the video's size
    auto ratio {m_graphicsVideoItem.boundingRect().width() / m_crosshair.boundingRect().width()};
    m_crosshair.setScale(m_crosshairScale * ratio);

    // Place lines at the video's center
    m_hLine.setPos(m_graphicsVideoItem.boundingRect().center());
    m_vLine.setPos(m_graphicsVideoItem.boundingRect().center());
    m_crosshair.setPos(m_graphicsVideoItem.boundingRect().center());

    // Show crosshair if enabled
    setCrosshairVisible(m_crosshairVisible);
    ui.graphicsView->fitInView(&m_graphicsVideoItem, Qt::KeepAspectRatio);
}

void Camera::resizeEvent(QResizeEvent *event)
{
    // Zoom to video
    ui.graphicsView->fitInView(&m_graphicsVideoItem, Qt::KeepAspectRatio);
}

QString Camera::generateFilePath()
{
    QDateTime dateTime {QDateTime::currentDateTime()};
    QString fileName {dateTime.toString("yyyy-MM-dd_hh-mm-ss.zzz") + '_' + m_cameraIdentifier};
    return m_recDir.absolutePath() + '/' + fileName;
}

void Camera::onScaleChanged(qreal scaleX, qreal scaleY)
{
    // Line positioning works relative to its center
    qreal videoHalfWidth = m_graphicsVideoItem.boundingRect().width() / 2 * scaleX;
    qreal videoHalfHeight = m_graphicsVideoItem.boundingRect().height() / 2 * scaleY;
    m_hLine.setLine(-videoHalfWidth, 0, videoHalfWidth, 0);
    m_vLine.setLine(0, -videoHalfHeight, 0, videoHalfHeight);
}
