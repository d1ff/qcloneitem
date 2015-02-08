#include "qcloneitem.h"
#include <QQuickWindow>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

class QCloneItemPrivate {
    QCloneItemPrivate(QCloneItem* q) : q(q)
    {
        // Initial settings
        m_thumbnailProperties.fSourceClientAreaOnly = false;
        m_thumbnailProperties.opacity = 255;
        m_thumbnailProperties.dwFlags =
            DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_VISIBLE |
            DWM_TNP_OPACITY | DWM_TNP_RECTDESTINATION;
        m_thumbnailProperties.fVisible = true;
        m_thumbnailProperties.rcDestination.left = 0;
        m_thumbnailProperties.rcDestination.top = 0;
        m_thumbnailProperties.rcDestination.right = 0;
        m_thumbnailProperties.rcDestination.bottom = 0;
    }

    bool enable();
    bool disable();

    HWND m_source;
    HWND m_destination;
    QCloneItem* q;
    HTHUMBNAIL m_thumbnail;
    DWM_THUMBNAIL_PROPERTIES m_thumbnailProperties;
    friend QCloneItem;
};

QCloneItem::QCloneItem(QQuickItem * parent) : QQuickItem(parent), d(new QCloneItemPrivate(this))
{
    connect(this, &QQuickItem::windowChanged, this, &QCloneItem::onWindowChanged);
    connect(this, &QQuickItem::opacityChanged, this, &QCloneItem::onOpacityChanged);
    connect(this, &QQuickItem::visibleChanged, this, &QCloneItem::onVisibleChanged);
    connect(this, &QCloneItem::thumbnailPropertiesChanged, this, &QCloneItem::onThumbnailPropertiesChanged);
    connect(this, &QCloneItem::targetWindowChanged, this, &QCloneItem::updateTargetWindow);
}

QCloneItem::~QCloneItem()
{
    d->disable();
}

void QCloneItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    QRect rect = newGeometry.toRect();
    rect.adjust(0, 0, 1, 1);
    
    if (rect.left()   == d->m_thumbnailProperties.rcDestination.left  &&
        rect.right()  == d->m_thumbnailProperties.rcDestination.right &&
        rect.top()    == d->m_thumbnailProperties.rcDestination.top   &&
        rect.bottom() == d->m_thumbnailProperties.rcDestination.bottom)
        return;

    d->m_thumbnailProperties.rcDestination.left   = rect.left();
    d->m_thumbnailProperties.rcDestination.right  = rect.right();
    d->m_thumbnailProperties.rcDestination.top    = rect.top();
    d->m_thumbnailProperties.rcDestination.bottom = rect.bottom();
    emit thumbnailPropertiesChanged();
}

void QCloneItem::onWindowChanged(QQuickWindow * window)
{
    if (!window)
        return;

    d->m_destination = (HWND)window->winId();
    d->enable();
}

void QCloneItem::onOpacityChanged()
{
    BYTE _opacity = (unsigned char)(opacity() * 255.0);
    if (d->m_thumbnailProperties.opacity == _opacity)
        return;

    d->m_thumbnailProperties.opacity = _opacity;
    emit thumbnailPropertiesChanged();
}

void QCloneItem::onVisibleChanged()
{
    if ((d->m_thumbnailProperties.fVisible != 0) == isVisible())
        return;

    d->m_thumbnailProperties.fVisible = isVisible();
    emit thumbnailPropertiesChanged();
}

bool QCloneItem::onThumbnailPropertiesChanged()
{
    if (!d->m_thumbnail)
        return false;

    return SUCCEEDED(DwmUpdateThumbnailProperties(d->m_thumbnail, &(d->m_thumbnailProperties)));
}

bool QCloneItemPrivate::disable()
{
    if (m_thumbnail)
    {
        HRESULT result = DwmUnregisterThumbnail(m_thumbnail);
        m_thumbnail = 0;
        return SUCCEEDED(result);
    }

    return true;
}

bool QCloneItemPrivate::enable()
{
    disable();

    BOOL isCompositionEnabled;
    if (!SUCCEEDED(DwmIsCompositionEnabled(&isCompositionEnabled)))
        return false;

    if (!isCompositionEnabled)
        return false;

    if (!SUCCEEDED(DwmRegisterThumbnail(m_destination, m_source, &m_thumbnail)))
        return false;

    if (!q->onThumbnailPropertiesChanged())
    {
        disable();
        return false;
    }

    SIZE size;
    if (!SUCCEEDED(DwmQueryThumbnailSourceSize(m_thumbnail, &size)))
        return false;

    m_thumbnailProperties.rcSource.left = 0;
    m_thumbnailProperties.rcSource.top = 0;
    m_thumbnailProperties.rcSource.right = size.cx;
    m_thumbnailProperties.rcSource.bottom = size.cy;
    emit q->thumbnailPropertiesChanged();
    return true;
}

void QCloneItem::setSourceClientAreaOnly(bool sourceClientAreaOnly)
{
    if ((d->m_thumbnailProperties.fSourceClientAreaOnly != 0) == sourceClientAreaOnly)
        return;

    d->m_thumbnailProperties.fSourceClientAreaOnly = sourceClientAreaOnly;
    emit thumbnailPropertiesChanged();
}

void QCloneItem::setSource(QRect source)
{
    if (source.left() == d->m_thumbnailProperties.rcSource.left    &&
        source.right() == d->m_thumbnailProperties.rcSource.right  &&
        source.top() == d->m_thumbnailProperties.rcSource.top      &&
        source.bottom() == d->m_thumbnailProperties.rcSource.bottom)
        return;

    d->m_thumbnailProperties.rcSource.left = source.left();
    d->m_thumbnailProperties.rcSource.right = source.right();
    d->m_thumbnailProperties.rcSource.top = source.top();
    d->m_thumbnailProperties.rcSource.bottom = source.bottom();

    if (!source.isValid())
    {
        // To go back to following the source size of the source window
        // we have to disable and enable without the rectsource flags.
        d->m_thumbnailProperties.dwFlags &= ~DWM_TNP_RECTSOURCE;
        d->enable();
    }
    else
    {
        d->m_thumbnailProperties.dwFlags |= DWM_TNP_RECTSOURCE;
        emit thumbnailPropertiesChanged();
    }
}

QRect QCloneItem::source()
{
    return QRect(
        QPoint(d->m_thumbnailProperties.rcSource.left, d->m_thumbnailProperties.rcSource.top),
        QPoint(d->m_thumbnailProperties.rcSource.right, d->m_thumbnailProperties.rcSource.bottom));
}

bool QCloneItem::sourceClientAreaOnly()
{
    return d->m_thumbnailProperties.fSourceClientAreaOnly;
}

void QCloneItem::updateTargetWindow()
{
    d->m_source = FindWindow(
        m_windowClass.isEmpty() ? nullptr : m_windowClass.toStdWString().data(),
        m_windowTitle.isEmpty() ? nullptr : m_windowTitle.toStdWString().data());

    d->enable();
}