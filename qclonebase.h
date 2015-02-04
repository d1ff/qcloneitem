#pragma once

#include <QtCore>
#include <qwindowdefs_win.h>
#include <memory>

class QCloneBasePrivate;

class QCloneBase
{
public:
    void setThumbnailOpacity(unsigned char opacity);
    unsigned char getThumbnailOpacity();

    void setThumbnailVisible(bool visible);
    bool getThumbnailVisible();

    void setSourceClientAreaOnly(bool sourceClientAreaOnly);
    bool getSourceClientAreaOnly();
    
    void setSource(QRect source);
    QRect getSource();

    void setDestination(QRect destination);
    QRect getDestination();

    bool setWindowToClone(QString windowClass, QString windowTitle);
    bool setWindowToClone(HWND handle);
    
protected:
    QCloneBase();
    virtual ~QCloneBase();

    virtual void propertiesChanged();

    bool enable(HWND target);
    bool disable();

    HWND m_hwnd;
    
private:
    bool updateThumbnailProperties();
    std::unique_ptr<QCloneBasePrivate> d;
    HWND m_target;
};