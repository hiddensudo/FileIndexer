#ifndef INDEXER_H
#define INDEXER_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

class Indexer {
private:
    std::string startDirectory;
    std::atomic<unsigned int> activeThreads;
    std::atomic<bool> isIndexingInCurrentDir;

    std::queue<std::string> directoryQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;

    bool toggleIndexingScope();
    void indexFilesBaseOnScope(const std::string currenctDirectory);
    void indexInDirAndSubdir(const std::string startDirectory);
    void indexInCurrentDir(const std::string startDirectory);
    void processDirecory();
    void sortByName();
    void sortByExtension();
    void sortByDate();
    void sortBySize();
    // void initList(std::string startDirectory);
public:
    Indexer(std::string startDirectory);
    ~Indexer() = default;

    void startIndexing();
};
#endif  // INDEXER_H