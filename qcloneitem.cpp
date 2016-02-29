#include "qcloneitem.h"

#include "qcloneitem_p.h"

QCloneItem::QCloneItem(QQuickItem * parent) : QQuickItem(parent), d_ptr(new QCloneItemPrivate(this))
{
}

QCloneItem::~QCloneItem()
{
}

void QCloneItem::updateTargetWindow()
{
    Q_D(QCloneItem);
    d->updateTargetWindow();
}

void QCloneItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QCloneItem);
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    d->setDestination(newGeometry);
}

void QCloneItem::componentComplete()
{
    Q_D(QCloneItem);
    QQuickItem::componentComplete();
    d->updateTargetWindow();
}

void QCloneItem::setWindowTitle(const QString & windowTitle)
{
    Q_D(QCloneItem);
    d->setWindowTitle(windowTitle);
    emit windowTitleChanged();
}

QString QCloneItem::windowTitle() const
{
    Q_D(const QCloneItem);
    return d->windowTitle();
}

void QCloneItem::setWindowClass(const QString & windowClass)
{
    Q_D(QCloneItem);
    d->setWindowClass(windowClass);
    emit windowClassChanged();
}

QString QCloneItem::windowClass() const
{
    Q_D(const QCloneItem);
    return d->windowClass();
}

void QCloneItem::setSourceClientAreaOnly(bool sourceClientAreaOnly)
{
    Q_D(QCloneItem);
    d->setSourceClientAreaOnly(sourceClientAreaOnly);
    emit sourceClientAreaOnlyChanged();
}

void QCloneItem::setSource(const QRect& source)
{
    Q_D(QCloneItem);
    d->setSource(source);
    emit sourceChanged();
}

QRect QCloneItem::source() const
{
    Q_D(const QCloneItem);
    return d->source();
}

bool QCloneItem::sourceClientAreaOnly() const
{
    Q_D(const QCloneItem);
    return d->sourceClientAreaOnly();
}
