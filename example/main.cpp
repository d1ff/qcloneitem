#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QDir applicationDirectory = QFileInfo(QGuiApplication::applicationFilePath()).absoluteDir();
    QString mainQml = applicationDirectory.absoluteFilePath(QStringLiteral("main.qml"));

    engine.load(mainQml);

    return app.exec();
}

