#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class Indexer
{
private:
    std::string startDirectory;
    std::atomic<unsigned int> activeThreads;

    std::queue<std::string> directoryQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;

    void indexFile(std::string startDirectory);
    void processDirecory();
    void sortByName();
    void sortByExtension();
    void sortByDate();
    void sortBySize();
    //void initList(std::string startDirectory);
public:
    Indexer(std::string startDirectory);
    ~Indexer() = default;

    void startIndexing();
};
#endif //INDEXER_H