#pragma once

#include <string>
#include <list>
#include <time.h>
#include <fstream>

enum class LOG_LEVEL {
    INFO,
    WARNING,
    ERROR
};

struct log_info
{
    std::string Time;
    LOG_LEVEL Log_Level;
    bool isGameEvent;
    std::string Message;
};


class Logger {
    public:
        static Logger& GetInstance() {
            if (s_instance == nullptr) {
                s_instance = new Logger;
            }

            return *s_instance;
        }
        void RenderLogger();
        
        void LogInfo(std::string);
        void LogGameEvent(std::string);
        void LogWarning(std::string);
        void LogGameWarning(std::string);
        void LogError(std::string);
        void LogGameError(std::string);
    private:
        static Logger* s_instance;
        std::ofstream LogFile;
        Logger();
        ~Logger();

        std::string GetTime();
        void PublishLogs();
        std::string ConstructLogString(const log_info& L);
        void SendLogToFile(const log_info& LL);
        std::list<log_info> log_bank;
};