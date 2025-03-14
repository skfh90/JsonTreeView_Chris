#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include "jsonmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    JsonModel *model = new JsonModel();
    QString currentDir = QDir::currentPath();
    qDebug() << currentDir;
    model->loadJson(currentDir + "/test.json");
    engine.rootContext()->setContextProperty("jsonModel", model);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("JSONTreeView", "Main");

    return app.exec();
}
