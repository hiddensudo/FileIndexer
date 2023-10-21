#include "MyApp.h"

void MyApp::setupAttributes() {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}

void MyApp::setupEngine() {
    const QUrl url(QStringLiteral("qrc:/UI/main.qml"));
    engine.rootContext()->setContextProperty("indexer", &indexer);
    engine.load(url);
}

void MyApp::setupConnections() {
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, this,
        [this](QObject *obj, const QUrl &objUrl) {
            if (!obj && objUrl == this->engine.rootContext()->baseUrl())
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
}

MyApp::MyApp(int &argc, char **argv)
    : QGuiApplication(argc, argv) {
    setupAttributes();
    setupEngine();
    setupConnections();
}