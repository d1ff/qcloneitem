#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include "qclonewindow.h"
#include "qcloneitem.h"


int main(int argc, char *argv[])
{
    QGuiApplication application(argc, argv);
    
    qmlRegisterType<QCloneItem>("QCloneItem", 1, 0, "Clone");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();

    QCloneWindow window;
    window.show();

    // start
    application.exec();
}