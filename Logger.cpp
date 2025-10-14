#include "Logger.h"
#include "imgui/imgui.h"
#include <iostream>
#include <chrono>

Logger* Logger::s_instance = nullptr;

Logger::Logger() {
    LogFile.open("logs.txt");
}

Logger::~Logger() {
    LogFile.close();
}

void Logger::RenderLogger() {

    ImGui::Begin("Game Log");

    if(ImGui::Button("Options")) {
        // TODO
    }
    ImGui::SameLine();

    if(ImGui::Button("Clear")) {
        log_bank.clear();
    }
    ImGui::SameLine();

    if(ImGui::Button("Test Info")) {
        LogInfo("This is a test info message");
    }
    ImGui::SameLine();

    if(ImGui::Button("Test Warning")) {
        LogWarning("This is a test warning message");
    }
    ImGui::SameLine();

    if(ImGui::Button("Test Error")) {
        LogError("This is a test error message");
    }

    ImGui::Separator();

    ImGui::BeginChild("Log Area");
    PublishLogs();
    ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::End();
}

// For the following 6 functions, I would have liked to just add a bool argument to cut the number of functions in half, but
// I needed this to work with the pre-done code on lines 18 and 19 of Application.cpp, so I went the worse option for now.
// In future projects I will be condensing this. 

void Logger::LogInfo(std::string S) {

    log_info temp_log;
        
    temp_log.Time = Logger::GetTime();
    temp_log.isGameEvent = false;
    temp_log.Log_Level = LOG_LEVEL::INFO;
    temp_log.Message = S;

    log_bank.push_back(temp_log);
    SendLogToFile(temp_log);
}

void Logger::LogGameEvent(std::string S) {
        
    log_info temp_log;
        
    temp_log.Time = Logger::GetTime();
    temp_log.isGameEvent = true;
    temp_log.Log_Level = LOG_LEVEL::INFO;
    temp_log.Message = S;

    log_bank.push_back(temp_log);
    SendLogToFile(temp_log);
}

void Logger::LogWarning(std::string S) {
        
    log_info temp_log;
        
    temp_log.Time = Logger::GetTime();
    temp_log.isGameEvent = false;
    temp_log.Log_Level = LOG_LEVEL::WARNING;
    temp_log.Message = S;

    log_bank.push_back(temp_log);
    SendLogToFile(temp_log);
}

void Logger::LogGameWarning(std::string S) {
        
    log_info temp_log;
        
    temp_log.Time = Logger::GetTime();
    temp_log.isGameEvent = true;
    temp_log.Log_Level = LOG_LEVEL::WARNING;
    temp_log.Message = S;

    log_bank.push_back(temp_log);
    SendLogToFile(temp_log);
}

void Logger::LogError(std::string S) {
    
    log_info temp_log;
        
    temp_log.Time = Logger::GetTime();
    temp_log.isGameEvent = false;
    temp_log.Log_Level = LOG_LEVEL::ERROR;
    temp_log.Message = S;

    log_bank.push_back(temp_log);
    SendLogToFile(temp_log);
}

void Logger::LogGameError(std::string S) {
    
    log_info temp_log;
        
    temp_log.Time = Logger::GetTime();
    temp_log.isGameEvent = true;
    temp_log.Log_Level = LOG_LEVEL::ERROR;
    temp_log.Message = S;

    log_bank.push_back(temp_log);
    SendLogToFile(temp_log);
}

std::string Logger::GetTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t t = system_clock::to_time_t(now);

    std::tm buf;
#if defined(_WIN32)
    localtime_s(&buf, &t);
#else
    localtime_r(&t, &buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&buf, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void Logger::PublishLogs() {

    for (const log_info& log_to_send : log_bank) {
        switch (log_to_send.Log_Level)
        {
        case LOG_LEVEL::INFO:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            break;

        case LOG_LEVEL::WARNING:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.65f, 0.0f, 1.0f));
            break;

        case LOG_LEVEL::ERROR:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            break;
        
        default:
            break;
        }

        ImGui::Text(ConstructLogString(log_to_send).c_str());

        ImGui::PopStyleColor();
    }
}

std::string Logger::ConstructLogString(const log_info& log_to_send) {
    std::string log_string;
    
    // add time
    log_string += "[";
    log_string += log_to_send.Time;
    log_string += "] ";

    // add log level
    log_string += "[";
    switch (log_to_send.Log_Level)
    {
    case LOG_LEVEL::INFO:
        log_string += "INFO";
        break;

    case LOG_LEVEL::WARNING:
        log_string += "WARN";
        break;

    case LOG_LEVEL::ERROR:
        log_string += "ERROR";
        break;
    
    default:
        break;
    }
    log_string += "] ";

    // add if its a game log
    if (log_to_send.isGameEvent) {
        log_string += "[";
        log_string += "GAME";
        log_string += "] ";
    }

    // add log message
    log_string += log_to_send.Message;
    log_string += '\n';

    return log_string;
}

void Logger::SendLogToFile(const log_info& logToSend) {
    LogFile << ConstructLogString(logToSend).c_str();
}