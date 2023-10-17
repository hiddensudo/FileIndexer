#ifndef PATHQUEUE_H
#define PATHQUEUE_H

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

class PathQueue {
private:
    std::atomic<unsigned int> activeThreads;
    std::atomic<bool> isProcessingInCurrentDir;

    std::mutex queueMutex;

    std::queue<std::filesystem::path> processingQueue;
    std::queue<std::filesystem::path> pathQueue;
    std::condition_variable queueCV;

    void addDirAndSubdir(std::string &currentDirectory);
    void addCurrentDir(std::string &currentDirectory);

    void processAll();
    void processDirectory(std::string &currentDirectory);
    void processQueue(std::string &currentDirectory);
    void processFilesBaseOnScope(std::string &currentDirectory);

    bool toggleIndexingScope();

public:
    PathQueue(std::string startDirectory);
    ~PathQueue() = default;

    void fillQueue(std::queue<std::filesystem::path> &queue);
};

#endif