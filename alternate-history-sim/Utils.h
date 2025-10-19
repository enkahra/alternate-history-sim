#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// ============= Shared Data Structures =============

struct FlagResult 
{
    std::string flag;           // e.g., "No Nuke"
    std::string matchedEvent;   // matched event key (e.g., "world war 2")
    std::string verb;           // detected verb/intent (e.g., "enter", "develop")
    double confidence = 0.0;    // simple confidence score [0..1] ? is this really needed
    std::vector<std::string> tags; // extra tags (e.g., negative phrase)
};

struct Event
{
    std::string name; // e.g, "World War II Begins"
    double probability; // base probability(0.0 - 1.0)
};















