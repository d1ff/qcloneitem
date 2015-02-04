#pragma once

#include <QWindow>
#include "qclonebase.h"

class QCloneWindow : public QWindow, public QCloneBase
{
    Q_OBJECT

    Q_PROPERTY(QRect source READ getSource WRITE setSource NOTIFY thumbnailPropertiesChanged)
    Q_PROPERTY(bool sourceClientAreaOnly READ getSourceClientAreaOnly WRITE setSourceClientAreaOnly NOTIFY thumbnailPropertiesChanged)

public:
    QCloneWindow(QWindow * parent = 0);
    ~QCloneWindow();

signals:
    void thumbnailPropertiesChanged();

protected:
    virtual void QCloneWindow::resizeEvent(QResizeEvent* event) override;
    virtual void propertiesChanged() override;

private:
    void onOpacityChanged();
    void onVisibleChanged();
};