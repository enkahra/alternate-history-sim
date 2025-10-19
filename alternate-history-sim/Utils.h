#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>



struct FlagResult {
    std::string flag;           // e.g., "No Nuke"
    std::string matchedEvent;   // matched event key (e.g., "world war 2")
    std::string verb;           // detected verb/intent (e.g., "enter", "develop")
    double confidence = 0.0;    // simple confidence score [0..1]
    std::vector<std::string> tags; // extra tags (e.g., negative phrase)
};

class FlagGenerator
{
private:
    std::unordered_set<std::string> negativeWords;
    std::unordered_map<std::string, std::string> eventKeywords;
    std::unordered_set<std::string> verbs; 
public:
    FlagGenerator();
    FlagResult generateFlag(const std::vector<std::string>& actionWords);
};









