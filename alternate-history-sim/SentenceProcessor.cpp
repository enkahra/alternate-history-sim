#include "SentenceProcessor.h"
#include <sstream>
#include <algorithm>
// For std::toupper
#include <cctype>

SentenceProcessor::SentenceProcessor(const std::string& input)
{
	std::istringstream iss(input);
	std::string word;
	while (iss >> word)
	{
		this->words.push_back(word);
	}
}

bool SentenceProcessor::isValid() const
{
	return !words.empty();
}


std::string SentenceProcessor::getFirstWord() const
{
	if (!isValid())
	{
		return "";
	}
	else
	{
		return words[0];
	}
}

std::vector<std::string> SentenceProcessor::getActionWords() const
{
	std::vector<std::string> actionWords;
	if (!isValid())
	{
		return actionWords; // Return empty vector
	}
	else
	{
		for (size_t i = 1; i < words.size(); i++)
		{
			actionWords.push_back(words[i]);
		}
	}
	return actionWords;
}

std::string SentenceProcessor::getCountryFormatted() const
{
	std::string country = getFirstWord();
	if (!country.empty())
	{
        // Use unsigned char to avoid UB for negative char values and cast result back to char
        std::transform(country.begin(), country.end(), country.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
	}
	return country;
}

