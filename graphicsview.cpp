#include "graphicsview.hpp"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QScrollBar>

GraphicsView::GraphicsView(QWidget *parent) : m_zoomFactor{1.0}
{
    setTransformationAnchor(ViewportAnchor::AnchorUnderMouse);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    auto angle = event->angleDelta().y();
    auto zoomFactor = 1 + angle / 1000.0;

    // Restrict zooming out too much
    auto currentScale = transform().m11();
    auto m_fitInViewScale = std::min(m_fitInViewScaleX, m_fitInViewScaleY);
    if (currentScale * zoomFactor < m_fitInViewScale)
    {
        zoomFactor = m_fitInViewScale / currentScale;
    }
    m_zoomFactor *= zoomFactor;

    scale(zoomFactor, zoomFactor);
    event->accept();
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::RightButton)
    {
        m_panStartX = event->position().x();
        m_panStartY = event->position().y();
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MouseButton::RightButton)
    {
        auto newHSB{horizontalScrollBar()->value() - event->position().x() + m_panStartX};
        auto newVSB{verticalScrollBar()->value() - event->position().y() + m_panStartY};
        horizontalScrollBar()->setValue(newHSB);
        verticalScrollBar()->setValue(newVSB);
        m_panStartX = event->position().x();
        m_panStartY = event->position().y();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRatioMode)
{
    auto viewportCenterScene = mapToScene(viewport()->rect().center());
    QGraphicsView::fitInView(item, aspectRatioMode);
    m_fitInViewScaleX = transform().m11();
    m_fitInViewScaleY = transform().m22();
    scale(m_zoomFactor, m_zoomFactor);
    centerOn(viewportCenterScene);
}

void GraphicsView::scale(qreal sx, qreal sy)
{
    QGraphicsView::scale(sx, sy);
    auto scaleX = m_fitInViewScaleX * m_zoomFactor;
    auto scaleY = m_fitInViewScaleY * m_zoomFactor;
    emit scaleChanged(scaleX, scaleY);
}
