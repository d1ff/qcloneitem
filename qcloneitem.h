#pragma once

#include <QQuickItem>
#include <memory>

class QCloneItemPrivate;

class QCloneItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QRect source READ getSource WRITE setSource NOTIFY thumbnailPropertiesChanged)
    Q_PROPERTY(bool sourceClientAreaOnly READ getSourceClientAreaOnly WRITE setSourceClientAreaOnly NOTIFY thumbnailPropertiesChanged)
    Q_PROPERTY(QString windowClass MEMBER m_windowClass NOTIFY targetWindowChanged)
    Q_PROPERTY(QString windowTitle MEMBER m_windowTitle NOTIFY targetWindowChanged)

public:
    QCloneItem(QQuickItem * parent = 0);
    ~QCloneItem();

signals:
    void thumbnailPropertiesChanged();
    void targetWindowChanged();

protected:
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    Q_INVOKABLE void updateTargetWindow();

private:
    void setSourceClientAreaOnly(bool sourceClientAreaOnly);
    bool getSourceClientAreaOnly();

    void setSource(QRect source);
    QRect getSource();

    bool enable(HWND target);
    bool disable();

    void onWindowChanged(QQuickWindow * window);
    void onOpacityChanged();
    void onVisibleChanged();
    bool updateThumbnailProperties();

    std::unique_ptr<QCloneItemPrivate> d;
    HWND m_target;
    HWND m_hwnd;

    QString m_windowClass;
    QString m_windowTitle;
};

