#ifndef PATHQUEUE_H
#define PATHQUEUE_H

#include <QObject>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

class PathQueue : public QObject {
    Q_OBJECT
private:

    std::atomic<unsigned int> activeThreads;
    bool isProcessingInCurrentDir;

    std::mutex queueMutex;

    std::queue<std::filesystem::path> processingQueue;
    std::queue<std::filesystem::path> pathQueue;
    std::condition_variable queueCV;

    void addDirAndSubdir(std::string currentDirectory);

    void processAll();
    void processDirectory(std::string currentDirectory);
    void processQueue(std::string &currentDirectory);
    void processFilesBaseOnScope(std::string currentDirectory);

    bool toggleIndexingScope();

public:
    PathQueue(std::string startDirectory, bool isProcessingInCurrentDir);
    ~PathQueue() = default;

    std::queue<std::filesystem::path> getQueue() const;
    void fillAndIndexQueue();

public slots:
    void detachRun();
    void fillQueue();
};

#endif