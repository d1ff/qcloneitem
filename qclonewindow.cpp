#include "qclonewindow.h"
#include <QResizeEvent>
#include "qclonebase.h"


QCloneWindow::QCloneWindow(QWindow * parent) : QWindow(parent)
{
    this->m_hwnd = (HWND)winId();
    connect(this, &QWindow::opacityChanged, this, &QCloneWindow::onOpacityChanged);
    connect(this, &QWindow::visibleChanged, this, &QCloneWindow::onVisibleChanged);

    QString title = "Steam";
    setWindowToClone(nullptr, title);
}

QCloneWindow::~QCloneWindow()
{

}

void QCloneWindow::onOpacityChanged()
{
    setThumbnailOpacity((unsigned char)(opacity() * 255.0));
}

void QCloneWindow::onVisibleChanged()
{
    setThumbnailVisible(isVisible());
}

void QCloneWindow::resizeEvent(QResizeEvent* event)
{
    QWindow::resizeEvent(event);
    QRect rect(0, 0, event->size().width(), event->size().height());
    rect.adjust(0, 0, 1, 1);
    setDestination(rect);
}

void QCloneWindow::propertiesChanged()
{
    QCloneBase::propertiesChanged();
    emit thumbnailPropertiesChanged();
}