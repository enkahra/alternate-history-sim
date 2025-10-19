#include <iostream>
#include <string>

// Includes: use relative paths to the library project
#include "../alternate-history-sim/SentenceProcessor.h"
#include "../alternate-history-sim/Utils.h"

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

    return 0;
}
