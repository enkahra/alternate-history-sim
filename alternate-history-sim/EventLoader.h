#pragma once

#include "Utils.h"
#include <string>
#include <unordered_map>

class EventLoader
{
private:
    std::string lastError;
    bool parseLine(const std::string& line, Event& outEvent);
public:
    EventLoader();
    std::unordered_map<std::string, Event> loadFromCSV(const std::string& filepath);
    std::string getLastError() const { return lastError; }
};