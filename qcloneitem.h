#pragma once

#include <QQuickItem>
#include "qclonebase.h"

class QCloneItem : public QQuickItem, public QCloneBase
{
    Q_OBJECT

    Q_PROPERTY(QRect source READ getSource WRITE setSource NOTIFY thumbnailPropertiesChanged)
    Q_PROPERTY(bool sourceClientAreaOnly READ getSourceClientAreaOnly WRITE setSourceClientAreaOnly NOTIFY thumbnailPropertiesChanged)

public:
    QCloneItem(QQuickItem * parent = 0);
    ~QCloneItem();

signals:
    void thumbnailPropertiesChanged();

protected:
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    virtual void propertiesChanged() override;

private:
    void onWindowChanged(QQuickWindow * window);
    void onOpacityChanged();
    void onVisibleChanged();
};

