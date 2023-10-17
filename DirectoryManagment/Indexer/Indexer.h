#ifndef INDEXER_H
#define INDEXER_H

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

#include "XMLWriter.h"

class Indexer : public QObject {
    Q_OBJECT
private:
    QString xmlPath;
    std::string startDirectory;
    std::atomic<unsigned int> activeThreads;
    std::atomic<bool> isIndexingInCurrentDir;

    std::queue<std::filesystem::path> directoryQueue;
    std::mutex indexMutex;
    std::condition_variable queueCV;

    bool toggleIndexingScope();
    void indexFiles(const std::string startDirectory);
    void processDirectory();

    void copyQueue(std::queue<std::filesystem::path> &directoryQueue);

    void writeInXml(const std::filesystem::directory_entry &entry);
    XMLWriter wr;

    void processAll();
    void processDirectory(std::string &currentDirectory);
    void processQueue(std::string &currentDirectory);

public:
    Indexer(std::string startDirectory);
    ~Indexer() = default;

public slots:
    void startIndexing();
};
#endif  // INDEXER_H