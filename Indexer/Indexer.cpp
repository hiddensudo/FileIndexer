#include "Indexer.h"

#include <filesystem>
#include <iostream>
#include <chrono>
#include <format>
#include <thread>
#include <vector>

void Indexer::startIndexing() {
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;

    std::vector<std::thread> threads;
    threads.reserve(maxThreads);

    for(unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&Indexer::processDirecory, this);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void Indexer::indexFile(std::string startDirectory) {
    try {
        if (!std::filesystem::exists(startDirectory)) {
            std::cout << "Directory does not exist: " << startDirectory <<  std::endl;
            return;
        }
        for (const auto& entry : std::filesystem::directory_iterator(startDirectory)) {
            if (entry.path() != "/proc" && entry.is_directory()) {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    this->directoryQueue.push(entry.path());

                    this->queueCV.notify_one();
                }
            } else if(entry.is_regular_file()) {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    //covert time for cout
                    auto fileTime = std::filesystem::last_write_time(entry.path());
                    auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(fileTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
                    std::time_t cftime = std::chrono::system_clock::to_time_t(systemTime);
                    
                    std::cout << "File name: " << entry.path().filename() << std::endl;
                    std::cout << "File extension: " << entry.path().extension() << std::endl;
                    std::cout << "File date: " << std::ctime(&cftime);
                    std::cout << "File size: " << std::filesystem::file_size(entry.path()) << " bytes" << std::endl;
                    std::cout << std::endl;

                    this->queueCV.notify_one();
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            std::cout << e.what() << std::endl;
        }
    }
}

void Indexer::processDirecory() {
    while (!(this->directoryQueue.empty() && this->activeThreads == 0)) {
        std::string currenctDirectory;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if(this->directoryQueue.empty()) {
                if(this->activeThreads == 0) {
                    queueCV.notify_all();
                    return;
                } else {
                    queueCV.wait(lock);
                }
            } else {
                currenctDirectory = directoryQueue.front();
                this->directoryQueue.pop();
            }
        }
        if(!currenctDirectory.empty()) {
            this->activeThreads++;

            indexFile(currenctDirectory);

            this->activeThreads--;

            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if(this->activeThreads == 0 && this->directoryQueue.empty()) {
                    queueCV.notify_all();
                    return;
                } else {
                    queueCV.notify_one();
                }
            }
        }
    }
}

Indexer::Indexer(std::string startDirectory) : activeThreads(0) {
    this->directoryQueue.push(startDirectory);
}