#ifndef PATHQUEUE_H
#define PATHQUEUE_H

#include <QObject>
#include <XMLWriter.h>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

class Indexer : public QObject {
    Q_OBJECT
private:
    QString xmlPath;
    XMLWriter wr;

    std::string startDirectory;

    bool isProcessingInCurrentDir;

    std::atomic<unsigned int> activeThreads;
    std::atomic<bool> isPaused;
    std::atomic<bool> isCancelled;
    std::atomic<bool> isStarted;
    std::mutex pauseMutex;

    std::queue<std::filesystem::path> processingQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;

    std::mutex indexMutex;

    void indexInDirAndSubDir(std::string currentDirectory);
    void indexInCurrentDir(std::string currentDirectory);

    void processAll();
    void processDirectory(std::string currentDirectory);
    void processQueue(std::string &currentDirectory);
    void processFilesBaseOnScope(std::string currentDirectory);

    void writeInXml(const std::filesystem::directory_entry &entry);

    bool toggleIndexingScope();

    void refresh();

public:
    Indexer(std::string startDirectory, bool isProcessingInCurrentDir);
    ~Indexer() = default;

    void fillAndIndexQueue();
    void startIndexing();

public slots:
    void detachRun();
    void pause();
    void resume();
    void stop();
};

#endif