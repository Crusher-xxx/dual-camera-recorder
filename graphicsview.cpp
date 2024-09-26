#include "graphicsview.hpp"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QScrollBar>

GraphicsView::GraphicsView(QWidget *parent) {
    setTransformationAnchor(ViewportAnchor::AnchorUnderMouse);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    auto angle = event->angleDelta().y();
    auto zoomFactor = 1 + angle / 1000.0;
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
