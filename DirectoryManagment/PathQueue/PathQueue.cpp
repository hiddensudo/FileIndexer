#include "PathQueue.h"

#include <iostream>
#include <thread>

void PathQueue::fillQueue(std::queue<std::filesystem::path>& queue) {
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;

    std::vector<std::thread> threads;
    threads.reserve(maxThreads);

    for (unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&PathQueue::processAll, this);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    std::cout << pathQueue.size() << std::endl;
    queue = this->pathQueue;
}

void PathQueue::addDirAndSubdir(std::string& currentDirectory) {
    try {
        if (!std::filesystem::exists(currentDirectory)) {
            std::cout << "Directory does not exist: " << currentDirectory
                      << std::endl;
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(currentDirectory)) {
            if (entry.path() != "/proc" && entry.is_directory()) {
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->processingQueue.push(entry.path());

                    this->pathQueue.push(entry.path());

                    this->queueCV.notify_one();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            std::cout << e.what() << std::endl;
        }
    }
}

void PathQueue::processAll() {
    while (!(this->processingQueue.empty() && this->activeThreads == 0)) {
        std::string currentDirectory;

        processQueue(currentDirectory);

        processDirectory(currentDirectory);
    }
}

void PathQueue::processQueue(std::string& currentDirectory) {
    std::unique_lock<std::mutex> lock(this->queueMutex);

    if (this->processingQueue.empty()) {
        if (this->activeThreads == 0) {
            queueCV.notify_all();
            return;
        } else {
            queueCV.wait(lock);
        }
    } else {
        currentDirectory = this->processingQueue.front();
        this->processingQueue.pop();
        std::cout << "Current PQ: " << currentDirectory << std::endl;
    }

    lock.unlock();
}

void PathQueue::processDirectory(std::string& currentDirectory) {
    if (!currentDirectory.empty()) {
        this->activeThreads++;

        processFilesBaseOnScope(currentDirectory);

        this->activeThreads--;

        {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            if (this->activeThreads == 0 && this->processingQueue.empty()) {
                queueCV.notify_all();
                return;
            } else {
                queueCV.notify_one();
            }
        }
    }
}

void PathQueue::processFilesBaseOnScope(std::string& currentDirectory) {
    if (!this->isProcessingInCurrentDir) {
        addDirAndSubdir(currentDirectory);
    } else {
        this->pathQueue.push(currentDirectory);
    }
}

PathQueue::PathQueue(std::string startDirectory)
    : activeThreads(0),
      isProcessingInCurrentDir(false),
      processingQueue(std::queue<std::filesystem::path>()),
      pathQueue(std::queue<std::filesystem::path>()) {
    this->processingQueue.push(startDirectory);
}