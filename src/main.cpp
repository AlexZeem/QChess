#include "logic.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qmlRegisterType<Logic>("Figure", 1, 0, "Figure");

    Logic logic;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("logic", &logic);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
