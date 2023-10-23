#include "Indexer.h"

#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <thread>

void Indexer::detachRun(const QString& startDirectory,
                        bool isProcessingInCurrentDir) {
    if (!this->isStarted) {
        this->startDirectory = startDirectory.toStdString();
        this->isProcessingInCurrentDir = isProcessingInCurrentDir;
        QtConcurrent::run([this] { startIndexing(); });
        qDebug() << "end";
    } else {
        qDebug() << "Already started";
    }
}

void Indexer::startIndexing() {
    refresh();
    qDebug() << "Started";
    this->isStarted = true;
    emit isStartedChanged();
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;

    std::vector<std::thread> threads;
    threads.reserve(maxThreads);

    for (unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&Indexer::processAll, this);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    this->isCancelled = true;
    emit isCancelledChanged();
    this->isStarted = false;
    emit indexingFinished();
    qDebug() << "Indexing done";
}

void Indexer::refresh() {
    this->isCancelled = false;
    this->isPaused = false;
    while (!this->processingQueue.empty()) {
        processingQueue.pop();
    }
    this->processingQueue.push(this->startDirectory);
    this->activeThreads = 0;
}

void Indexer::pause() {
    if (this->isStarted && !this->isPaused) {
        this->isPaused = true;
        qDebug() << "Paused" << isPaused;
        emit isPausedChanged();
    } else {
        qDebug() << "Indexing was not started, cannot pause";
    }
}

void Indexer::resume() {
    if (this->isStarted && this->isPaused) {
        this->isPaused = false;
        qDebug() << "Paused" << isPaused;
        emit isPausedChanged();
    } else {
        qDebug() << "Indexing was not started, cannot resume";
    }
}

void Indexer::stop() {
    if (this->isStarted && !this->isCancelled) {
        this->isCancelled.store(true, std::memory_order_relaxed);
        emit isCancelledChanged();
        qDebug() << "Stoped";
    } else {
        qDebug() << "Indexing was not started, cannot stop";
    }
}

bool Indexer::getIsStarted() const { return isStarted; }
bool Indexer::getIsPaused() const { return isPaused; }
bool Indexer::getIsCancelled() const { return isCancelled; }

QString convertTime(const std::filesystem::directory_entry& entry) {
    std::filesystem::file_time_type fileTime =
        std::filesystem::last_write_time(entry);

    auto sctp =
        std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            fileTime - std::filesystem::file_time_type::clock::now() +
            std::chrono::system_clock::now());

    std::time_t systemTime = std::chrono::system_clock::to_time_t(sctp);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&systemTime), "%F %T");

    return QString::fromStdString(ss.str());
}

void Indexer::writeInXml(const std::filesystem::directory_entry& entry) {
    QString name = QString::fromStdString(entry.path().filename().string());

    QString extension =
        QString::fromStdString(entry.path().extension().string());

    QString date = convertTime(entry);

    QString size = QString::number(std::filesystem::file_size(entry.path()));
    wr.writeInFile("XMLDataBase", name, extension, date, size);
}

bool isDirectoryExistst(const std::string& currentDirectory) {
    if (std::filesystem::exists(currentDirectory)) {
        return true;
    } else {
        qDebug() << "Directory does not exist: " << currentDirectory;
        return false;
    }
}

void Indexer::indexInDirAndSubDir(const std::string& currentDirectory) {
    try {
        if (!isDirectoryExistst) {
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(currentDirectory)) {
            if (entry.path() != "/proc" && entry.is_directory()) {
                {
                    if (this->isCancelled) {
                        return;
                    } else {
                        {
                            std::unique_lock<std::mutex> lock(this->queueMutex);

                            this->processingQueue.push(entry.path());

                            this->queueCV.notify_one();
                        }
                    }
                }
            } else if (entry.is_regular_file()) {
                if (this->isCancelled) {
                    return;
                } else {
                    {
                        std::unique_lock<std::mutex> lock(this->indexMutex);

                        writeInXml(entry);

                        this->queueCV.notify_one();
                    }
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::unique_lock<std::mutex> lock(this->queueMutex);
        std::cout << e.what() << std::endl;
        lock.unlock();
    }
}

void Indexer::indexInCurrentDir(const std::string &currentDirectory) {
    try {
        if (!std::filesystem::exists(currentDirectory)) {
            std::cout << "Directory does not exist: " << currentDirectory
                      << std::endl;
            return;
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(currentDirectory)) {
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

void Indexer::processAll() {
    while (!(this->processingQueue.empty() && this->activeThreads == 0)) {
        if (this->isCancelled) {
            return;
        }

        if (this->isPaused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        } else {
            std::string currentDirectory;

            processQueue(currentDirectory);

            processDirectory(currentDirectory);
        }
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

void Indexer::processDirectory(const std::string& currentDirectory) {
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
        qDebug() << this->activeThreads;
    }
}

void Indexer::processFilesBaseOnScope(const std::string& currentDirectory) {
    if (this->isProcessingInCurrentDir) {
        indexInCurrentDir(currentDirectory);
    } else {
        indexInDirAndSubDir(currentDirectory);
    }
}

Indexer::Indexer()
    : xmlPath("db.xml"),
      wr("../" + xmlPath),
      startDirectory(),
      isProcessingInCurrentDir(true),
      activeThreads(0),
      isPaused(false),
      isCancelled(false),
      isStarted(false) {}
