#ifndef MyApp_H
#define MyApp_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent/QtConcurrent>

#include "Indexer.h"

class MyApp : public QGuiApplication {
private:
    QQmlApplicationEngine engine;
    Indexer indexer;

    void setupAttributes();
    void setupEngine();
    void setupConnections();
public:
    MyApp(int &argc, char **argv);

    int run() { return exec(); }
};

#endif