#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include <QtWidgets/QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);

private:
    qreal m_panStartX;
    qreal m_panStartY;
    qreal m_zoomFactor;
    qreal m_fitInViewScale;
};

#endif // GRAPHICSVIEW_HPP
