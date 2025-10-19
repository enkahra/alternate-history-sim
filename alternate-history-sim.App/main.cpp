#include <iostream>
#include <string>
#include <iomanip>

// Includes
#include "../alternate-history-sim/SentenceProcessor.h"
#include "../alternate-history-sim/Utils.h"
#include "../alternate-history-sim/FlagGenerator.h"
#include "../alternate-history-sim/EventLoader.h"

int main()
{
    std::cout << "Enter English input (e.g., 'usa atom bomb does not develop'):" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    SentenceProcessor sentence(input);

    if (sentence.isValid())
    {
        std::cout << "Formatted Country: " << sentence.getCountryFormatted() << std::endl;

        // Generate and print flag based on action words
        FlagGenerator flagGen;
        FlagResult res = flagGen.generateFlag(sentence.getActionWords());
        if (!res.flag.empty())
        {
            std::cout << "Generated Flag: " << res.flag << std::endl;
            if (!res.matchedEvent.empty()) std::cout << "Matched Event: " << res.matchedEvent << std::endl;
            if (!res.verb.empty()) std::cout << "Detected Verb: " << res.verb << std::endl;
            std::cout << "Confidence: " << res.confidence << std::endl;
            if (!res.tags.empty()) {
                std::cout << "Tags:";
                for (const auto &t : res.tags) std::cout << " " << t;
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "No flag generated. Confidence: " << res.confidence << std::endl;
        }
    }
    else
    {
        std::cout << "No valid input provided!" << std::endl;
    }

    // ============= Day 3: CSV Loading =============
    std::cout << "\n--- Loading Historical Events from CSV ---" << std::endl;

    // Define CSV path (relative to executable location)
    // Try multiple paths to handle different working directories
    std::string CSV_PATH = "events.csv";  // First try: same directory as executable
    
    // Create EventLoader and load events
    EventLoader loader;
    auto eventMap = loader.loadFromCSV(CSV_PATH);
    
    // If not found, try relative paths
    if (eventMap.empty()) {
        CSV_PATH = "../../data/events.csv";  // From x64/Debug to solution root
        eventMap = loader.loadFromCSV(CSV_PATH);
    }
    
    if (eventMap.empty()) {
        CSV_PATH = "../data/events.csv";  // Alternative path
        eventMap = loader.loadFromCSV(CSV_PATH);
    }

    // Check if loading succeeded
    if (eventMap.empty())
    {
        std::cerr << "Error: " << loader.getLastError() << std::endl;
        std::cerr << "Note: CSV file should be at: " << CSV_PATH << std::endl;
    }
    else
    {
        std::cout << "Successfully loaded " << eventMap.size() << " events." << std::endl;
        std::cout << "\nHistorical Events:" << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        // Print each event with formatted probability
        for (const auto& pair : eventMap)
        {
            std::cout << "Event: " << pair.second.name
                      << " - Probability: " << std::fixed << std::setprecision(2)
                      << pair.second.probability << std::endl;
        }
        std::cout << std::string(50, '-') << std::endl;
    }

    return 0;
}
