#include <QtQml/QQmlExtensionPlugin>
#include "qcloneitem.h"

class QCloneItemPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        qmlRegisterType<QCloneItem>(uri, 1, 0, "Clone");
    }
};

#include "qcloneitemplugin.moc"