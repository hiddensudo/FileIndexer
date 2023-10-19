#ifndef MyApp_H
#define MyApp_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent/QtConcurrent>

#include "PathQueue.h"

class MyApp : public QGuiApplication {
private:
    QQmlApplicationEngine engine;
    PathQueue pathQueue;

    void setupAttributes();
    void setupEngine();
    void setupConnections();
public:
    MyApp(int &argc, char **argv);

    int run() { return exec(); }
};

#endif