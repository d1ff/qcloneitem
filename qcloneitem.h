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

#pragma once

#include <QQuickItem>
#include <QQuickWindow>

class QCloneItemPrivate;

class QCloneItem : public QQuickItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCloneItem)
    Q_PROPERTY(QRect source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool sourceClientAreaOnly READ sourceClientAreaOnly WRITE setSourceClientAreaOnly NOTIFY sourceClientAreaOnlyChanged)
    Q_PROPERTY(QString windowClass READ windowClass WRITE setWindowClass NOTIFY windowClassChanged)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged)
	Q_PROPERTY(QQuickWindow* windowTemplate READ windowTemplate WRITE setWindowTemplate NOTIFY windowTemplateChanged)

public:
    QCloneItem(QQuickItem * parent = 0);
    virtual ~QCloneItem() override;

Q_SIGNALS:
    void sourceChanged();
    void sourceClientAreaOnlyChanged();
    void windowTitleChanged();
    void windowClassChanged();
	void windowTemplateChanged();

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

	void setWindowTemplate(QQuickWindow* windowTemplate);
	QQuickWindow* windowTemplate() const;

    const QScopedPointer<QCloneItemPrivate> d_ptr;
};
