#include "Indexer.h"

#include <chrono>
#include <format>
#include <iostream>
#include <thread>
#include <vector>

#include "XMLWriter.h"

void Indexer::startIndexing() {
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;

    std::vector<std::thread> threads;
    threads.reserve(maxThreads);

    for (unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&Indexer::processDirecory, this);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

bool Indexer::toggleIndexingScope() {
    return this->isIndexingInCurrentDir = !this->isIndexingInCurrentDir;
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
    return QString::fromStdString(std::ctime(&cftime));
}

void Indexer::writeInXml(const std::filesystem::directory_entry& entry) {
    QString name = QString::fromStdString(entry.path().filename().string());

    QString extension =
        QString::fromStdString(entry.path().extension().string());

    QString date = convertTime(entry);

    QString size = QString::number(std::filesystem::file_size(entry.path()));
    XMLWriter wr("../test.xml");

    wr.writeInFile("XMLDataBase", name, extension, date, size);
}

void Indexer::indexInCurrentDir(const std::string startDirectory) {
    try {
        if (!std::filesystem::exists(startDirectory)) {
            std::cout << "Directory does not exist: " << startDirectory
                      << std::endl;
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(startDirectory)) {
            if (entry.is_regular_file()) {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    writeInXml(entry);

                    this->queueCV.notify_one();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            std::cout << e.what() << std::endl;
        }
    }
}

void Indexer::indexInDirAndSubdir(const std::string startDirectory) {
    try {
        if (!std::filesystem::exists(startDirectory)) {
            std::cout << "Directory does not exist: " << startDirectory
                      << std::endl;
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(startDirectory)) {
            if (entry.path() != "/proc" && entry.is_directory()) {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    this->directoryQueue.push(entry.path());

                    this->queueCV.notify_one();
                }
            } else if (entry.is_regular_file()) {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    writeInXml(entry);

                    this->queueCV.notify_one();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            std::cout << e.what() << std::endl;
        }
    }
}

void Indexer::processDirecory() {
    while (!(this->directoryQueue.empty() && this->activeThreads == 0)) {
        std::string currentDirectory;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (this->directoryQueue.empty()) {
                if (this->activeThreads == 0) {
                    queueCV.notify_all();
                    return;
                } else {
                    queueCV.wait(lock);
                }
            } else {
                currentDirectory = directoryQueue.front();
                this->directoryQueue.pop();
            }
        }
        if (!currentDirectory.empty()) {
            this->activeThreads++;

            indexFilesBaseOnScope(currentDirectory);

            this->activeThreads--;

            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (this->activeThreads == 0 && this->directoryQueue.empty()) {
                    queueCV.notify_all();
                    return;
                } else {
                    queueCV.notify_one();
                }
            }
        }
    }
}

void Indexer::indexFilesBaseOnScope(const std::string currentDirectory) {
    if (this->isIndexingInCurrentDir) {
        indexInCurrentDir(currentDirectory);
    } else {
        indexInDirAndSubdir(currentDirectory);
    }
}

Indexer::Indexer(std::string startDirectory)
    : activeThreads(0), isIndexingInCurrentDir(true) {
    this->directoryQueue.push(startDirectory);
}