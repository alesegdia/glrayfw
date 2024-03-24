#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERR
};

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template<typename... Args>
    void log(LogLevel level, Args... args) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&now_time), "[%Y-%m-%d %H:%M:%S]");
        std::ostringstream oss;
        oss << time_ss.str() << " ";
        logHelper(oss, args...);
        printLog(level, oss.str());
    }

private:
    std::ofstream outFile;
    std::string filename;
    std::mutex mutex;

    Logger() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&now_time), "%Y%m%d%H%M%S");
        std::string timestamp = time_ss.str();
        filename = "logfile_" + timestamp + ".txt";

        outFile.open(filename, std::ios::out | std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open log file " << filename << std::endl;
        }
    }

    ~Logger() {
        if (outFile.is_open()) {
            outFile.close();
        }
    }

    template<typename T, typename... Args>
    void logHelper(std::ostringstream& oss, T first, Args... args) {
        oss << first << " ";
        logHelper(oss, args...);
    }

    void logHelper(std::ostringstream& oss) {}

    void printLog(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        if (outFile.is_open()) {
            switch (level) {
            case LogLevel::INFO:
                outFile << "[INFO] " << message << std::endl;
                break;
            case LogLevel::WARNING:
                outFile << "[WARNING] " << message << std::endl;
                break;
            case LogLevel::ERR:
                outFile << "[ERROR] " << message << std::endl;
                break;
            default:
                std::cerr << "Unknown log level" << std::endl;
            }
            outFile.flush(); // Flush the output stream
        }
        else {
            std::cerr << "Error: Log file is not open." << std::endl;
        }

        // Print to standard output
        switch (level) {
        case LogLevel::INFO:
            std::cout << "[INFO] " << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] " << message << std::endl;
            break;
        case LogLevel::ERR:
            std::cout << "[ERROR] " << message << std::endl;
            break;
        default:
            std::cerr << "Unknown log level" << std::endl;
        }
    }
};