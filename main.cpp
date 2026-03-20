#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "weatherbackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    WeatherBackend weatherBackend;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("weatherBackend", &weatherBackend);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return QGuiApplication::exec();
}