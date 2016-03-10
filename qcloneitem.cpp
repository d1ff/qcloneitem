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

#include "qcloneitem.h"

#include "qcloneitem_p.h"

QCloneItem::QCloneItem(QQuickItem * parent) : QQuickItem(parent), d_ptr(new QCloneItemPrivate(this))
{
}

QCloneItem::~QCloneItem()
{
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
}

void QCloneItem::setSource(const QRect& source)
{
    Q_D(QCloneItem);
    d->setSource(source);
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
