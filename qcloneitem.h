#pragma once

#include <QQuickItem>

class QCloneItemPrivate;

class QCloneItem : public QQuickItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCloneItem)
    Q_PROPERTY(QRect source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool sourceClientAreaOnly READ sourceClientAreaOnly WRITE setSourceClientAreaOnly NOTIFY sourceClientAreaOnlyChanged)
    Q_PROPERTY(QString windowClass READ windowClass WRITE setWindowClass NOTIFY windowClassChanged)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged)
        
public:
    QCloneItem(QQuickItem * parent = 0);
    virtual ~QCloneItem();

    Q_INVOKABLE void updateTargetWindow();

Q_SIGNALS:
    void sourceChanged();
    void sourceClientAreaOnlyChanged();
    void windowTitleChanged();
    void windowClassChanged();

protected:
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    virtual void componentComplete() override;

private:
    void setWindowTitle(const QString& windowTitle);
    QString windowTitle() const;

    void setWindowClass(const QString& windowClass);
    QString windowClass() const;

    void setSourceClientAreaOnly(bool sourceClientAreaOnly);
    bool sourceClientAreaOnly() const;

    void setSource(const QRect& source);
    QRect source() const;

    const QScopedPointer<QCloneItemPrivate> d_ptr;
};

