#ifndef INDEXER_H
#define INDEXER_H

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

#include "XMLWriter.h"

class Indexer {
private:
    QString xmlPath;
    std::string startDirectory;
    std::atomic<unsigned int> activeThreads;
    std::atomic<bool> isIndexingInCurrentDir;

    std::queue<std::filesystem::path> directoryQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;

    bool toggleIndexingScope();
    void indexFilesBaseOnScope(const std::string currenctDirectory);
    void indexInDirAndSubdir(const std::string startDirectory);
    void indexInCurrentDir(const std::string startDirectory);
    void processDirecory();

    void writeInXml(const std::filesystem::directory_entry& entry);
    XMLWriter wr;

public:
    Indexer(std::string startDirectory);
    ~Indexer() = default;

    void startIndexing();
};
#endif  // INDEXER_H