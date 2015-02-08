#pragma once

#include <QQuickItem>
#include <memory>

class QCloneItemPrivate;

class QCloneItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QRect source READ source WRITE setSource NOTIFY thumbnailPropertiesChanged)
    Q_PROPERTY(bool sourceClientAreaOnly READ sourceClientAreaOnly WRITE setSourceClientAreaOnly NOTIFY thumbnailPropertiesChanged)
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
    bool sourceClientAreaOnly();

    void setSource(QRect source);
    QRect source();

    void onWindowChanged(QQuickWindow * window);
    void onOpacityChanged();
    void onVisibleChanged();
    bool onThumbnailPropertiesChanged();

    std::unique_ptr<QCloneItemPrivate> d;

    QString m_windowClass;
    QString m_windowTitle;

    friend class QCloneItemPrivate;
};

