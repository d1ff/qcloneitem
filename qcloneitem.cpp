#include "qcloneitem.h"
#include <QQuickWindow>

QCloneItem::QCloneItem(QQuickItem * parent) : QQuickItem(parent)
{
    connect(this, &QQuickItem::windowChanged, this, &QCloneItem::onWindowChanged);
    connect(this, &QQuickItem::opacityChanged, this, &QCloneItem::onOpacityChanged);
    connect(this, &QQuickItem::visibleChanged, this, &QCloneItem::onVisibleChanged);
}

QCloneItem::~QCloneItem()
{

}

void QCloneItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    QRect rect = newGeometry.toRect();
    rect.adjust(0, 0, 1, 1);
    setDestination(rect);
}

void QCloneItem::onWindowChanged(QQuickWindow * window)
{
    if (!window)
        return;

    m_hwnd = (HWND)window->winId();
    QString title = "Steam";
    setWindowToClone(nullptr, title);
}

void QCloneItem::onOpacityChanged()
{
    setThumbnailOpacity((unsigned char)(opacity() * 255.0));
}

void QCloneItem::onVisibleChanged()
{
    setThumbnailVisible(isVisible());
}

void QCloneItem::propertiesChanged()
{
    QCloneBase::propertiesChanged();
    emit thumbnailPropertiesChanged();
}