#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <QQuickWindow>

class QCloneItemPrivate: public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QCloneItem)
public:
    QCloneItemPrivate(QCloneItem* q) : QObject(q), q_ptr(q)
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

        connect(q, &QQuickItem::windowChanged, this,  &QCloneItemPrivate::onWindowChanged);
        connect(q, &QQuickItem::opacityChanged, this, &QCloneItemPrivate::onOpacityChanged);
        connect(q, &QQuickItem::visibleChanged, this, &QCloneItemPrivate::onVisibleChanged);
    }

    ~QCloneItemPrivate()
    {
        disable();
    }
private:

    void onWindowChanged(QQuickWindow * window)
    {
        if (!window)
            return;

        m_destination = (HWND)window->winId();
        enable();
    }

    void onOpacityChanged()
    {
        Q_Q(QCloneItem);
        BYTE _opacity = (unsigned char)(q->opacity() * 255.0);
        if (m_thumbnailProperties.opacity != _opacity)
        {
            m_thumbnailProperties.opacity = _opacity;
            updateThumbnailProperties();
        }
    }

    void onVisibleChanged()
    {
        Q_Q(QCloneItem);
        bool visible = q->isVisible();
        if ((m_thumbnailProperties.fVisible != 0) != visible)
        {
            m_thumbnailProperties.fVisible = visible;
            updateThumbnailProperties();
        }
    }

    QRect source() const 
    {
        return QRect(
            QPoint(m_thumbnailProperties.rcSource.left, m_thumbnailProperties.rcSource.top),
            QPoint(m_thumbnailProperties.rcSource.right, m_thumbnailProperties.rcSource.bottom));
    }

    void setSource(QRect _source)
    {
        if (_source == source())
            return;

        m_thumbnailProperties.rcSource.left   = _source.left();
        m_thumbnailProperties.rcSource.right  = _source.right();
        m_thumbnailProperties.rcSource.top    = _source.top();
        m_thumbnailProperties.rcSource.bottom = _source.bottom();

        if (!_source.isValid())
        {
            // To go back to following the source size of the source window
            // we have to disable and enable without the rectsource flags.
            m_thumbnailProperties.dwFlags &= ~DWM_TNP_RECTSOURCE;
            enable();
        }
        else
        {
            m_thumbnailProperties.dwFlags |= DWM_TNP_RECTSOURCE;
            updateThumbnailProperties();
        }
    }

    QRect destination() const 
    {
        return QRect(
            QPoint(m_thumbnailProperties.rcDestination.left, m_thumbnailProperties.rcDestination.top),
            QPoint(m_thumbnailProperties.rcDestination.right, m_thumbnailProperties.rcDestination.bottom));
    }

    void setDestination(QRectF _destination)
    {
        QRect rect = _destination.toRect();
        rect.adjust(0, 0, 1, 1);
    
        if (rect == destination())
            return;

        m_thumbnailProperties.rcDestination.left   = rect.left();
        m_thumbnailProperties.rcDestination.right  = rect.right();
        m_thumbnailProperties.rcDestination.top    = rect.top();
        m_thumbnailProperties.rcDestination.bottom = rect.bottom();
        updateThumbnailProperties();
    }

    void setSourceClientAreaOnly(bool sourceClientAreaOnly)
    {
        if ((m_thumbnailProperties.fSourceClientAreaOnly != 0) != sourceClientAreaOnly)
        {
            m_thumbnailProperties.fSourceClientAreaOnly = sourceClientAreaOnly;
            updateThumbnailProperties();
        }
    }

    void updateTargetWindow()
    {
        m_source = FindWindow(
            m_windowClass.isEmpty() ? nullptr : m_windowClass.toStdWString().data(),
            m_windowTitle.isEmpty() ? nullptr : m_windowTitle.toStdWString().data());

        enable();
    }

    bool sourceClientAreaOnly() const
    {
        return m_thumbnailProperties.fSourceClientAreaOnly;
    }

    void updateThumbnailProperties()
    {
        if (!m_thumbnail)
            return;

        DwmUpdateThumbnailProperties(m_thumbnail, &m_thumbnailProperties);
    }

    bool disable()
    {
        if (m_thumbnail)
        {
            HRESULT result = DwmUnregisterThumbnail(m_thumbnail);
            m_thumbnail = 0;
            return SUCCEEDED(result);
        }
    
        return true;
    }

    bool enable()
    {
        disable();
    
        BOOL isCompositionEnabled;
        if (!SUCCEEDED(DwmIsCompositionEnabled(&isCompositionEnabled)))
            return false;
    
        if (!isCompositionEnabled)
            return false;
    
        if (!SUCCEEDED(DwmRegisterThumbnail(m_destination, m_source, &m_thumbnail)))
            return false;
    
        SIZE size;
        if (!SUCCEEDED(DwmQueryThumbnailSourceSize(m_thumbnail, &size)))
            return false;
    
        m_thumbnailProperties.rcSource.left = 0;
        m_thumbnailProperties.rcSource.top = 0;
        m_thumbnailProperties.rcSource.right = size.cx;
        m_thumbnailProperties.rcSource.bottom = size.cy;
        updateThumbnailProperties();
        return true;
    }

    void setWindowTitle(const QString& windowTitle)
    {
        m_windowTitle = windowTitle;
        updateTargetWindow();
    }

    QString windowTitle() const
    {
        return m_windowTitle;
    }

    void setWindowClass(const QString& windowClass)
    {
        m_windowClass = windowClass;
        updateTargetWindow();
    }

    QString windowClass() const
    {
        return m_windowClass;
    }

    HWND m_source;
    HWND m_destination;
    QCloneItem* q_ptr;
    HTHUMBNAIL m_thumbnail;
    DWM_THUMBNAIL_PROPERTIES m_thumbnailProperties;

    QString m_windowClass;
    QString m_windowTitle;
};
