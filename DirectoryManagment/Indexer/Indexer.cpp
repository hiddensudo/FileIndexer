#include "Indexer.h"

#include <chrono>
#include <format>
#include <iostream>
#include <thread>
#include <vector>

#include "PathQueue.h"

void Indexer::startIndexing() {
    copyQueue(this->directoryQueue);
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;

    std::vector<std::thread> threads;
    threads.reserve(maxThreads);

    for (unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&Indexer::processAll, this);
    }

    for (auto& thread : threads) {
        thread.detach();
    }
}

void Indexer::copyQueue(std::queue<std::filesystem::path>& directoryQueue) {
    PathQueue q(this->startDirectory);
    q.fillQueue(directoryQueue);
}

std::time_t convertToTimeT(const std::filesystem::directory_entry& entry) {
    auto fileTime = std::filesystem::last_write_time(entry.path());
    auto systemTime =
        std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            fileTime - std::filesystem::file_time_type::clock::now() +
            std::chrono::system_clock::now());
    return std::chrono::system_clock::to_time_t(systemTime);
}

QString convertTime(const std::filesystem::directory_entry& entry) {
    std::time_t cftime = convertToTimeT(entry);
    return QString::fromStdString(std::ctime(&cftime)).trimmed();
}

void Indexer::writeInXml(const std::filesystem::directory_entry& entry) {
    QString name = QString::fromStdString(entry.path().filename().string());

    QString extension =
        QString::fromStdString(entry.path().extension().string());

    QString date = convertTime(entry);

    QString size = QString::number(std::filesystem::file_size(entry.path()));
    wr.writeInFile("XMLDataBase", name, extension, date, size);
}

bool isDirectoryExistst(const std::string currentDirectory) {
    if (std::filesystem::exists(currentDirectory)) {
        return true;
    } else {
        std::cout << "Directory does not exist: " << currentDirectory
                  << std::endl;
        return false;
    }
}

void Indexer::indexFiles(const std::string currentDirectory) {
    try {
        if (!isDirectoryExistst) {
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(currentDirectory)) {
            if (entry.is_regular_file()) {
                {
                    std::unique_lock<std::mutex> lock(this->indexMutex);

                    writeInXml(entry);

                    this->queueCV.notify_one();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        {
            std::unique_lock<std::mutex> lock(this->indexMutex);
            std::cout << e.what() << std::endl;
        }
    }
}

void Indexer::processAll() {
    while (!(this->directoryQueue.empty() && this->activeThreads == 0)) {
        std::string currentDirectory;

        processQueue(currentDirectory);

        processDirectory(currentDirectory);
    }
}

void Indexer::processQueue(std::string& currentDirectory) {
    std::unique_lock<std::mutex> lock(this->indexMutex);

    if (this->directoryQueue.empty()) {
        if (this->activeThreads == 0) {
            queueCV.notify_all();
            return;
        } else {
            queueCV.wait(lock);
        }
    } else {
        currentDirectory = this->directoryQueue.front();
        this->directoryQueue.pop();
        std::cout << "Current Indexer: " << currentDirectory << std::endl;
    }

    lock.unlock();
}

void Indexer::processDirectory(std::string& currentDirectory) {
    if (!currentDirectory.empty()) {
        this->activeThreads++;

        indexFiles(currentDirectory);

        this->activeThreads--;

        {
            std::unique_lock<std::mutex> lock(this->indexMutex);
            if (this->activeThreads == 0 && this->directoryQueue.empty()) {
                queueCV.notify_all();
                return;
            } else {
                queueCV.notify_one();
            }
        }
    }
}

bool Indexer::toggleIndexingScope() {
    return this->isIndexingInCurrentDir = !this->isIndexingInCurrentDir;
}

Indexer::Indexer(std::string startDirectory)
    : activeThreads(0),
      startDirectory(startDirectory),
      isIndexingInCurrentDir(true),
      directoryQueue(std::queue<std::filesystem::path>()),
      xmlPath("db.xml"),
      wr("../" + xmlPath) {
    this->directoryQueue.push(startDirectory);
}
