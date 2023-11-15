#include "MyApp.h"

void MyApp::setupAttributes() {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}

void MyApp::setupEngine() {
    engine.rootContext()->setContextProperty("indexer", &indexer);
    engine.rootContext()->setContextProperty("fillModel", &fillModel);

    fillModel.parseXML();

    engine.rootContext()->setContextProperty("myModel", fillModel.getModel());

    const QUrl url(QStringLiteral("qrc:/UI/main.qml"));
    engine.load(url);
}

void MyApp::setupConnections() { // Should be disconnected in destructor
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, this,
        [this](const QObject *obj, const QUrl &objUrl) {
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