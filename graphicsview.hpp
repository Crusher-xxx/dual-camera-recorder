#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include <QtWidgets/QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

signals:
    void scaleChanged(qreal scaleX, qreal scaleY);

public:
    GraphicsView(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);
    void scale(qreal sx, qreal sy);

private:
    qreal m_panStartX;
    qreal m_panStartY;
    qreal m_zoomFactor;
    qreal m_fitInViewScaleX;
    qreal m_fitInViewScaleY;
};

#endif // GRAPHICSVIEW_HPP
