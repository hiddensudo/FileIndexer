#ifndef MyApp_H
#define MyApp_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent/QtConcurrent>

#include "Indexer.h"
#include "FillModel.h"

class MyApp : public QGuiApplication {
private:
    QQmlApplicationEngine engine;
    Indexer indexer;
    FillModel fillModel;

    void setupAttributes();
    void setupEngine();
    void setupConnections();
public:
    // Why do we path by reference?
    // It can be modified?
    MyApp(int &argc, char **argv);

    int run() { return exec(); }
};

#endif