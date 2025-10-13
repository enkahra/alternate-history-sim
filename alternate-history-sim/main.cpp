#include <iostream>
#include <string>
// Local header for sentence processing
#include "SentenceProcessor.h"

int main()
{
    std::cout << "Enter English input (e.g., 'usa atom bomb does not develop'):" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    SentenceProcessor sentence(input);

    if (sentence.isValid())
    {
        std::cout << "Formatted Country: " << sentence.getCountryFormatted() << std::endl;
    }
    else
    {
        std::cout << "No valid input provided!" << std::endl;
    }

    return 0;
}
