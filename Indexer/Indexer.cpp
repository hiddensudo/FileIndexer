#include "Indexer.h"

#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <thread>

void Indexer::detachRun() {
    if(!this->isStarted) {
        QtConcurrent::run([this] { startIndexing(); });
    } else {
        qDebug() << "Already started";
    }
}

void Indexer::startIndexing() {
    refresh();
    qDebug() << "Started";
    this->isStarted = true;
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;

    std::vector<std::thread> threads;
    threads.reserve(maxThreads);

    for (unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&Indexer::processAll, this);
    }

    while (!this->isCancelled) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (this->isCancelled) {
            break; 
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    qDebug() << "Indexing done";

    isStarted = false;
}

void Indexer::refresh() {
    this->isCancelled = false;
    this->isPaused = false;
    while (this->processingQueue.empty()) {
        processingQueue.pop();
    }
    this->processingQueue.push(this->startDirectory);
    this->activeThreads = 0;
}

void Indexer::pause() {
    if(this->isStarted) {
        this->isPaused.store(true, std::memory_order_relaxed);
        qDebug() << "Paused";
    } else {
        qDebug() << "Indexing was not started, cannot pause";
    }
}

void Indexer::resume() {
    if (this->isStarted) {
        this->isPaused.store(false, std::memory_order_relaxed);
        qDebug() << "Resumed";
    } else {
        qDebug() << "Indexing was not started, cannot resume";
    }
}

void Indexer::stop() {
    if (this->isStarted) {
        this->isCancelled.store(true, std::memory_order_relaxed);
        qDebug() << "Stoped";
    } else {
        qDebug() << "Indexing was not started, cannot stop";
    }
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
        qDebug() << "Directory does not exist: " << currentDirectory;
        return false;
    }
}

void Indexer::indexInDirAndSubDir(std::string currentDirectory) {
    try {
        if (!isDirectoryExistst) {
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(currentDirectory)) {
            if (entry.path() != "/proc" && entry.is_directory()) {
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);

                    this->processingQueue.push(entry.path());

                    this->queueCV.notify_one();
                }
            } else if (entry.is_regular_file()) {
                {
                    std::unique_lock<std::mutex> lock(this->indexMutex);

                    writeInXml(entry);

                    this->queueCV.notify_one();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::unique_lock<std::mutex> lock(this->queueMutex);
        std::cout << e.what() << std::endl;
        lock.unlock();
    }
}

void Indexer::indexInCurrentDir(const std::string currentDirectory) {
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
            qDebug() << e.what();
        }
    }
}

void Indexer::processAll() {
    while (!(this->processingQueue.empty() && this->activeThreads == 0)) {
        std::string currentDirectory;

        if (this->isCancelled) {
            return;
        }

        if (this->isPaused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        processQueue(currentDirectory);

        processDirectory(currentDirectory);
    }
}

void Indexer::processQueue(std::string& currentDirectory) {
    {
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
            qDebug() << "Current dir" << currentDirectory;
        }
    }
}

void Indexer::processDirectory(std::string currentDirectory) {
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

void Indexer::processFilesBaseOnScope(std::string currentDirectory) {
    if (!this->isProcessingInCurrentDir) {
        indexInDirAndSubDir(currentDirectory);
    } else {
        this->processingQueue.push(currentDirectory);
    }
}

Indexer::Indexer(std::string startDirectory, bool isProcessingInCurrentDir)
    : activeThreads(0),
      isProcessingInCurrentDir(isProcessingInCurrentDir),
      xmlPath("db.xml"),
      wr("../" + xmlPath),
      isPaused(false),
      isCancelled(false),
      isStarted(false) {
    this->processingQueue.push(startDirectory);
}