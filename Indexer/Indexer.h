#ifndef PATHQUEUE_H
#define PATHQUEUE_H

#include <XMLWriter.h>

#include <QObject>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

class Indexer : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isStarted READ getIsStarted NOTIFY isStartedChanged)
    Q_PROPERTY(bool isPaused READ getIsPaused NOTIFY isPausedChanged)
    Q_PROPERTY(bool isCancelled READ getIsCancelled NOTIFY isCancelledChanged)
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

    void indexInDirAndSubDir(const std::string &currentDirectory);
    void indexInCurrentDir(const std::string &currentDirectory);

    void processAll();
    void processDirectory(const std::string &currentDirectory);
    void processQueue(std::string &currentDirectory);
    void processFilesBaseOnScope(const std::string &currentDirectory);

    void writeInXml(const std::filesystem::directory_entry &entry);

    bool toggleIndexingScope();

    void refresh();
    void startIndexing();

public:
    Indexer();
    ~Indexer() = default;

    Q_INVOKABLE void detachRun(const QString &startDirectory,
                               bool isProcessingInCurrentDir);

    bool getIsStarted() const;
    bool getIsPaused() const;
    bool getIsCancelled() const;

signals:
    void isStartedChanged();
    void isPausedChanged();
    void isCancelledChanged();

public slots:

    void pause();
    void resume();
    void stop();
};

#endif