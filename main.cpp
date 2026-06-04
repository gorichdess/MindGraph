#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "GraphController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    GraphController graphController;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("graphController", &graphController);
    engine.rootContext()->setContextProperty("notesModel", graphController.notesModel());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );

    engine.loadFromModule("MindGraph", "Main");

    return app.exec();
}
