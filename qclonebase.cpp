#include "qclonebase.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

class QCloneBasePrivate {
    HTHUMBNAIL m_thumbnail;
    DWM_THUMBNAIL_PROPERTIES m_thumbnailProperties;
    friend QCloneBase;
};

QCloneBase::QCloneBase() : d(new QCloneBasePrivate())
{
    // Initial settings
    d->m_thumbnailProperties.fSourceClientAreaOnly = false;
    d->m_thumbnailProperties.opacity = 255;
    d->m_thumbnailProperties.dwFlags = 
        DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_VISIBLE |
        DWM_TNP_OPACITY | DWM_TNP_RECTDESTINATION;
    d->m_thumbnailProperties.fVisible = true;
    d->m_thumbnailProperties.rcDestination.left = 0;
    d->m_thumbnailProperties.rcDestination.top = 0;
    d->m_thumbnailProperties.rcDestination.right = 0;
    d->m_thumbnailProperties.rcDestination.bottom = 0;
}

QCloneBase::~QCloneBase()
{
    disable();
}

bool QCloneBase::updateThumbnailProperties()
{
    if (!d->m_thumbnail)
        return false;

    return SUCCEEDED(DwmUpdateThumbnailProperties(d->m_thumbnail, &(d->m_thumbnailProperties)));
}

bool QCloneBase::disable()
{
    if (d->m_thumbnail)
    {
        HRESULT result = DwmUnregisterThumbnail(d->m_thumbnail);
        d->m_thumbnail = 0;
        return SUCCEEDED(result);
    }

    return true;
}

bool QCloneBase::enable(HWND target)
{
    // Clear old thumbnail
    disable();

    // Check if compositing is available
    BOOL isCompositionEnabled;
    DwmIsCompositionEnabled(&isCompositionEnabled);
    if (!isCompositionEnabled)
        return false;

    if (!SUCCEEDED(DwmRegisterThumbnail(m_hwnd, target, &d->m_thumbnail)))
        return false;

    if (!updateThumbnailProperties())
    {
        disable();
        return false;
    }

    SIZE size;
    DwmQueryThumbnailSourceSize(d->m_thumbnail, &size);
    d->m_thumbnailProperties.rcSource.left = 0;
    d->m_thumbnailProperties.rcSource.top = 0;
    d->m_thumbnailProperties.rcSource.right = size.cx;
    d->m_thumbnailProperties.rcSource.bottom = size.cy;

    return true;
}

void QCloneBase::setThumbnailVisible(bool visible)
{
    if ((d->m_thumbnailProperties.fVisible != 0) == visible)
        return;

    d->m_thumbnailProperties.fVisible = visible;
    propertiesChanged();
}

void QCloneBase::setSourceClientAreaOnly(bool sourceClientAreaOnly)
{
    if ((d->m_thumbnailProperties.fSourceClientAreaOnly != 0) == sourceClientAreaOnly)
        return;

    d->m_thumbnailProperties.fSourceClientAreaOnly = sourceClientAreaOnly;
    propertiesChanged();
}

void QCloneBase::setThumbnailOpacity(BYTE opacity)
{
    if (d->m_thumbnailProperties.opacity == opacity)
        return;

    d->m_thumbnailProperties.opacity = opacity;
    propertiesChanged();
}

void QCloneBase::setSource(QRect source)
{
    if (source.left()   == d->m_thumbnailProperties.rcSource.left    &&
        source.right()  == d->m_thumbnailProperties.rcSource.right  &&
        source.top()    == d->m_thumbnailProperties.rcSource.top      &&
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
        enable(m_target);
    }
    else
    {
        d->m_thumbnailProperties.dwFlags |= DWM_TNP_RECTSOURCE;
        propertiesChanged();
    }
}

void QCloneBase::setDestination(QRect destination)
{
    if (destination.left()   == d->m_thumbnailProperties.rcDestination.left    &&
        destination.right()  == d->m_thumbnailProperties.rcDestination.right  &&
        destination.top()    == d->m_thumbnailProperties.rcDestination.top      &&
        destination.bottom() == d->m_thumbnailProperties.rcDestination.bottom)
        return;

    d->m_thumbnailProperties.rcDestination.left = destination.left();
    d->m_thumbnailProperties.rcDestination.right = destination.right();
    d->m_thumbnailProperties.rcDestination.top = destination.top();
    d->m_thumbnailProperties.rcDestination.bottom = destination.bottom();
    propertiesChanged();
}


QRect QCloneBase::getDestination() 
{ 
    return QRect(
        QPoint(d->m_thumbnailProperties.rcDestination.left,  d->m_thumbnailProperties.rcDestination.top),
        QPoint(d->m_thumbnailProperties.rcDestination.right, d->m_thumbnailProperties.rcDestination.bottom));
}

QRect QCloneBase::getSource()
{
    return QRect(
        QPoint(d->m_thumbnailProperties.rcSource.left,  d->m_thumbnailProperties.rcSource.top),
        QPoint(d->m_thumbnailProperties.rcSource.right, d->m_thumbnailProperties.rcSource.bottom));
}

unsigned char QCloneBase::getThumbnailOpacity() 
{ 
    return d->m_thumbnailProperties.opacity; 
}

bool QCloneBase::getThumbnailVisible() 
{ 
    return d->m_thumbnailProperties.fVisible; 
}

bool QCloneBase::getSourceClientAreaOnly() 
{ 
    return d->m_thumbnailProperties.fSourceClientAreaOnly; 
}

bool QCloneBase::setWindowToClone(QString windowClass, QString windowTitle)
{
    m_target = FindWindow(
        windowClass != nullptr ? windowClass.toLocal8Bit().constData() : nullptr, 
        windowTitle != nullptr ? windowTitle.toLocal8Bit().constData() : nullptr);

    return enable(m_target);
}

bool QCloneBase::setWindowToClone(HWND handle)
{
    m_target = handle;
    return enable(m_target);
}

void QCloneBase::propertiesChanged()
{
    updateThumbnailProperties();
}