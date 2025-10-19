#pragma once

#include "Utils.h"



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