#include "EventLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

// Helper function to manually trim whitespace (avoids linker issues with find_first_not_of/find_last_not_of)
static std::string trim(const std::string& str) {
	size_t start = 0;
	size_t end = str.length();
	
	// Trim leading whitespace
	while (start < end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n')) {
		start++;
	}
	
	// Trim trailing whitespace
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t' || str[end - 1] == '\r' || str[end - 1] == '\n')) {
		end--;
	}
	
	return str.substr(start, end - start);
}

// Helper function to check if string is only whitespace
static bool isWhitespaceOnly(const std::string& str) {
	for (char c : str) {
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			return false;
		}
	}
	return true;
}

EventLoader::EventLoader() : lastError("") {}

std::unordered_map<std::string, Event> EventLoader::loadFromCSV(const std::string& filepath)
{
	std::unordered_map<std::string, Event> eventMap;
	std::ifstream file(filepath);

	if (!file.is_open())
	{
		lastError = "Failed to open file: " + filepath;
		return eventMap; // Return empty map
	}

	// Clear any previous errors since file opened successfully
	lastError.clear();

	std::string headerLine;
	std::getline(file, headerLine); // Skip header line (assumes first line is "name,probability")

	std::string line;
	int lineNumber = 1; // Track line for error reporting

	while (std::getline(file, line))
	{
		lineNumber++;
		
		// Skip empty lines - use helper function to avoid linker issues
		if (line.empty() || isWhitespaceOnly(line))
			continue;

		Event event;

		if (parseLine(line, event))
		{
			// Store with lowercase key for case-insensitive matching
			std::string key = event.name;
			std::transform(key.begin(), key.end(), key.begin(),
				[](unsigned char c) { return std::tolower(c); });

			eventMap[key] = event;
		}
		else
		{
			// Store warning but continue processing
			lastError = "Warning: Malformed line " + std::to_string(lineNumber) + ": " + line;
		}
	}

	file.close();

	// Clear error if we successfully loaded at least some events
	if (!eventMap.empty() && !lastError.empty() && lastError.find("Warning") != std::string::npos)
	{
		lastError.clear();
	}

	return eventMap;
}

bool EventLoader::parseLine(const std::string& line, Event& outEvent)
{
	if (line.empty()) return false;

	// Find the last comma (in case event name contains commas)
	size_t lastComma = line.find_last_of(',');
	if (lastComma == std::string::npos)
		return false; // No comma found

	// Split into name and probability
	std::string nameField = line.substr(0, lastComma);
	std::string probabilityField = line.substr(lastComma + 1);

	// Trim whitespace using helper function (avoids linker issues)
	nameField = trim(nameField);
	probabilityField = trim(probabilityField);

	if (nameField.empty() || probabilityField.empty())
		return false;

	// Parse probability
	try
	{
		outEvent.name = nameField;
		outEvent.probability = std::stod(probabilityField);

		// Validate probability range (0.0 to 1.0)
		if (outEvent.probability < 0.0 || outEvent.probability > 1.0)
		{
			return false; // Probability out of valid range
		}

		return true;
	}
	catch (const std::invalid_argument&)
	{
		return false; // Not a valid number
	}
	catch (const std::out_of_range&)
	{
		return false; // Number too large
	}
}
