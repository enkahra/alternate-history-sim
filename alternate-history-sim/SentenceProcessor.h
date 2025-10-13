#pragma once

#include <string>
#include <vector>



class SentenceProcessor 
{
public:
	SentenceProcessor(const std::string& input);

	std::string getFirstWord() const; // This one dont change country name to upper

	std::string getCountryFormatted() const;

	std::string getActionWords() const; // This is for other words, implement later
	
	bool isValid() const;

private:
	std::vector<std::string> words;
};




