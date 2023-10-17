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
    MyApp(int &argc, char **argv)
        : QGuiApplication(argc, argv), indexer("/home/hidden") {
       setupAttributes();
       setupEngine();
       setupConnections();
    }

    int run() { return exec(); }
};

#endif