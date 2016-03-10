/*
  Copyright (c) 2015-2016, Jesper Hellesø Hansen
  jesperhh@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <QQuickWindow>

namespace {
    QString WindowClassFromHwnd(HWND hwnd)
    {
        const int windowClassLength = 256; // max length according to MSDN
        const QScopedArrayPointer<wchar_t> windowClass(new wchar_t[windowClassLength]);
        GetClassName(hwnd, windowClass.data(), windowClassLength);
        return QString::fromWCharArray(windowClass.data());
    }

    QString WindowTitleFromHwnd(HWND hwnd)
    {
        const int windowTextLength = GetWindowTextLength(hwnd) + 1;
        const QScopedArrayPointer<wchar_t> windowText(new wchar_t[windowTextLength]);
        GetWindowText(hwnd, windowText.data(), windowTextLength);
        return QString::fromWCharArray(windowText.data());
    }
}

class WinEventHookWrapper: public QObject
{
    Q_OBJECT
public:
    inline static WinEventHookWrapper& instance()
    {
        static WinEventHookWrapper winEventHookWrapper;
        return winEventHookWrapper;
    }

Q_SIGNALS:
    void windowCreated(const QString& windowClass, const QString& windowText, HWND windowHandle);
    void windowResized(HWND windowHandle);

private:
    WinEventHookWrapper() : hWinEventHook(0)
    {
        hWinEventHook = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_NAMECHANGE, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    }

    ~WinEventHookWrapper()
    {
        if (hWinEventHook)
            UnhookWinEvent(hWinEventHook);
    }

    static void __stdcall WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
    {
        Q_UNUSED(dwmsEventTime);
        Q_UNUSED(dwEventThread);
        Q_UNUSED(hWinEventHook);

        if (!(hwnd && idObject == OBJID_WINDOW && idChild == CHILDID_SELF && IsWindow(hwnd)))
            return;

        if (event == EVENT_OBJECT_CREATE || event == EVENT_OBJECT_NAMECHANGE)
        {
            const QString windowText = WindowTitleFromHwnd(hwnd);
            const QString windowClass = WindowClassFromHwnd(hwnd);
            emit instance().windowCreated(windowClass, windowText, hwnd);
        }
        else if (event == EVENT_OBJECT_LOCATIONCHANGE)
        {
            emit instance().windowResized(hwnd);
        }
    }

    HWINEVENTHOOK hWinEventHook;
};

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
        connect(&WinEventHookWrapper::instance(), &WinEventHookWrapper::windowCreated, this, &QCloneItemPrivate::onWindowCreated);
        connect(&WinEventHookWrapper::instance(), &WinEventHookWrapper::windowResized, this, &QCloneItemPrivate::onWindowResized);
    }

    ~QCloneItemPrivate()
    {
        disable();
    }
private:
    void onWindowCreated(const QString& windowClass, const QString& windowText, HWND windowHandle)
    {
        if (m_windowClass == windowClass && m_windowTitle == windowText)
        {
            setSource(windowHandle);
        }
    }

    void onWindowResized(HWND window)
    {
        if ((window == m_source || window == m_actualSource) && m_actualSource != m_source)
        {
            setSource(matchSourceToChildWindow());
        }
    }

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
        const BYTE _opacity = (unsigned char)(q->opacity() * 255.0);
        if (m_thumbnailProperties.opacity != _opacity)
        {
            m_thumbnailProperties.opacity = _opacity;
            updateThumbnailProperties();
        }
    }

    void onVisibleChanged()
    {
        Q_Q(QCloneItem);
        const bool visible = q->isVisible();
        if ((m_thumbnailProperties.fVisible != 0) != visible)
        {
            m_thumbnailProperties.fVisible = visible;
            updateThumbnailProperties();
        }
    }

    QRect source() const
    {
        return QRect(
            QPoint(m_thumbnailProperties.rcSource.left,  m_thumbnailProperties.rcSource.top),
            QPoint(m_thumbnailProperties.rcSource.right, m_thumbnailProperties.rcSource.bottom));
    }

    void setSource(const QRect& _source)
    {
        Q_Q(QCloneItem);
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

        setDestination(q->boundingRect());
        q->sourceChanged();
    }

    QRect destination() const
    {
        return QRect(
            QPoint(m_thumbnailProperties.rcDestination.left,  m_thumbnailProperties.rcDestination.top),
            QPoint(m_thumbnailProperties.rcDestination.right, m_thumbnailProperties.rcDestination.bottom));
    }

    void setDestination(const QRectF& _destination)
    {
        Q_Q(QCloneItem);

        QRect dst = q->mapRectToScene(_destination).toRect();
        dst.adjust(0, 0, 1, 1);

        const QRect src = source();
        const double scale = std::min(
            (double)dst.width() / (double)src.width(),
            (double)dst.height() / (double)src.height());

        dst.setWidth(src.width() * scale);
        dst.setHeight(src.height() * scale);

        if (dst == destination())
            return;

        m_thumbnailProperties.rcDestination.left   = dst.left();
        m_thumbnailProperties.rcDestination.right  = dst.right();
        m_thumbnailProperties.rcDestination.top    = dst.top();
        m_thumbnailProperties.rcDestination.bottom = dst.bottom();
        q->setImplicitSize(dst.width(), dst.height());
        updateThumbnailProperties();
    }

    void setSourceClientAreaOnly(bool sourceClientAreaOnly)
    {
        Q_Q(QCloneItem);
        if ((m_thumbnailProperties.fSourceClientAreaOnly != 0) != sourceClientAreaOnly)
        {
            m_thumbnailProperties.fSourceClientAreaOnly = sourceClientAreaOnly;
            updateThumbnailProperties();
            emit q->sourceClientAreaOnlyChanged();
        }
    }

    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
    {
        QCloneItemPrivate* that = (QCloneItemPrivate*)lParam;

        if (that->m_windowClass == WindowClassFromHwnd(hwnd) &&
            that->m_windowTitle == WindowTitleFromHwnd(hwnd))
        {
            that->setSource(hwnd);
            return false;
        }

        return true;
    }

    void updateTargetWindow(HWND parent = GetDesktopWindow())
    {
        if (m_windowClass.isEmpty())
            return;

        EnumChildWindows(parent, EnumChildProc, (LPARAM)this);
    }

    void setSource(HWND source)
    {
        if (source == m_source)
            return;

        m_source = m_actualSource = source;
        if (HWND root = GetAncestor(m_source, GA_ROOT))
        {
            m_actualSource = root;
            setSource(matchSourceToChildWindow());
        }

        enable();
    }

    QRect matchSourceToChildWindow()
    {
        RECT rect;
        GetClientRect(m_source, &rect);

        POINT corners[2];
        corners[0].x = rect.left;
        corners[0].y = rect.top;
        corners[1].x = rect.right;
        corners[1].y = rect.bottom;

        MapWindowPoints(m_source, m_actualSource, &corners[0], 2);
        return QRect(QPoint(corners[0].x, corners[0].y), QPoint(corners[1].x, corners[1].y));
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

        if (!SUCCEEDED(DwmRegisterThumbnail(m_destination, m_actualSource, &m_thumbnail)))
            return false;

        SIZE size;
        if (!SUCCEEDED(DwmQueryThumbnailSourceSize(m_thumbnail, &size)))
            return false;

        if (m_actualSource == m_source)
        {
            m_thumbnailProperties.rcSource.left = 0;
            m_thumbnailProperties.rcSource.top = 0;
            m_thumbnailProperties.rcSource.right = size.cx;
            m_thumbnailProperties.rcSource.bottom = size.cy;
        }

        updateThumbnailProperties();
        return true;
    }

    void setWindowTitle(const QString& windowTitle)
    {
        Q_Q(QCloneItem);
        if (m_windowTitle != windowTitle)
        {
            m_windowTitle = windowTitle;
            updateTargetWindow();
            emit q->windowTitleChanged();
        }
    }

    QString windowTitle() const
    {
        return m_windowTitle;
    }

    void setWindowClass(const QString& windowClass)
    {
        Q_Q(QCloneItem);
        if (m_windowClass != windowClass)
        {
            m_windowClass = windowClass;
            updateTargetWindow();
            emit q->windowClassChanged();
        }
    }

    QString windowClass() const
    {
        return m_windowClass;
    }

    HWND m_source;
    HWND m_actualSource;
    HWND m_destination;
    QCloneItem* q_ptr;
    HTHUMBNAIL m_thumbnail;
    DWM_THUMBNAIL_PROPERTIES m_thumbnailProperties;

    QString m_windowClass;
    QString m_windowTitle;
};
