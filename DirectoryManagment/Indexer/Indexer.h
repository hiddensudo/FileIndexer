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
    XMLWriter wr;

    std::atomic<unsigned int> activeThreads;
    std::atomic<bool> isIndexingInCurrentDir;

    std::queue<std::filesystem::path> directoryQueue;
    std::mutex indexMutex;
    std::condition_variable queueCV;

    bool toggleIndexingScope();
    void indexFiles(const std::string startDirectory);
    void processDirectory();


    void writeInXml(const std::filesystem::directory_entry &entry);

    void processAll();
    void processDirectory(std::string &currentDirectory);
    void processQueue(std::string &currentDirectory);

public:
    Indexer(std::queue<std::filesystem::path> queue);
    ~Indexer() = default;

public slots:
    void startIndexing();
};
#endif  // INDEXER_H