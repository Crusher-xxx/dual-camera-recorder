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
    auto currentScale = transform().m11();
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
    m_fitInViewScale = transform().m11();
    scale(m_zoomFactor, m_zoomFactor);
    centerOn(viewportCenterScene);
}
