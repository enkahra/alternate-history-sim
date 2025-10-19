#include "pch.h" // This is for precompiled headers, leave it in.

// The key step: Include the header from your main project.
// The ".." means "go up one directory level".
#include "../alternate-history-sim/SentenceProcessor.h"
#include "../alternate-history-sim/Utils.h"
#include "../alternate-history-sim/FlagGenerator.h"
#include "../alternate-history-sim/EventLoader.h"
#include <fstream>

// Test that the raw first word is retrieved correctly
TEST(SentenceProcessorTest, CorrectlyGetsFirstWordRaw) {
    SentenceProcessor sentence("uSa atom bomb");
    EXPECT_EQ("uSa", sentence.getFirstWord());
}

// Test that the formatted country name is correct
TEST(SentenceProcessorTest, CorrectlyGetsCountryFormatted) {
    SentenceProcessor sentence("uSa atom bomb");
    EXPECT_EQ("USA", sentence.getCountryFormatted());
}

TEST(SentenceProcessorTest, CorrectlyGetsCountryFormatted2) {
    SentenceProcessor sentence("otTOmAn collapse");
    EXPECT_EQ("OTTOMAN", sentence.getCountryFormatted());
}


// Test that the remaining action words are correct
TEST(SentenceProcessorTest, CorrectlyGetsActionWords) {
    SentenceProcessor sentence("usa atom bomb does not develop");
    std::vector<std::string> expected = { "atom", "bomb", "does", "not", "develop" };
    EXPECT_EQ(expected, sentence.getActionWords());
}

// Test an edge case: only one word in the input
TEST(SentenceProcessorTest, HandlesSingleWordInput) {
    SentenceProcessor sentence("Japan");
    EXPECT_EQ("JAPAN", sentence.getCountryFormatted());
    // Action words vector should be empty
    EXPECT_TRUE(sentence.getActionWords().empty());
}

// Test an edge case: empty input
TEST(SentenceProcessorTest, HandlesEmptyInput) {
    SentenceProcessor sentence("");
    EXPECT_FALSE(sentence.isValid());
    EXPECT_EQ("", sentence.getCountryFormatted());
    EXPECT_TRUE(sentence.getActionWords().empty());
}

// Tests for FlagGenerator
TEST(FlagGeneratorTest, GeneratesFlagForDoesNotDevelop) {
    FlagGenerator gen;
    std::vector<std::string> words = { "atom", "bomb", "does", "not", "develop" };
    FlagResult res = gen.generateFlag(words);
    EXPECT_EQ("No Nuke", res.flag);
}

TEST(FlagGeneratorTest, GeneratesFlagForNeverWar) {
    FlagGenerator gen;
    std::vector<std::string> words = { "war", "never", "happens" };
    FlagResult res = gen.generateFlag(words);
    EXPECT_EQ("No War", res.flag);
}

TEST(FlagGeneratorTest, GeneratesFlagForEmpireDies) {
    FlagGenerator gen;
    std::vector<std::string> words = { "empire", "dies" };
    FlagResult res = gen.generateFlag(words);
    EXPECT_EQ("No Empire", res.flag);
}

TEST(FlagGeneratorTest, NoFlagWhenNoNegative) {
    FlagGenerator gen;
    std::vector<std::string> words = { "atom", "bomb", "develops" };
    FlagResult res = gen.generateFlag(words);
    EXPECT_EQ("", res.flag);
}

TEST(FlagGeneratorTest, NoFlagWhenNoEvent) {
    FlagGenerator gen;
    std::vector<std::string> words = { "something", "does", "not", "happen" };
    FlagResult res = gen.generateFlag(words);
    EXPECT_EQ("", res.flag);
}

// Regression test: ensure entity selection prefers the event near the verb/negation
TEST(FlagGeneratorTest, OttomanDoesNotEnterWW2) {
    FlagGenerator gen;
    std::vector<std::string> words = { "empire", "does", "not", "enter", "ww2" };
    FlagResult res = gen.generateFlag(words);
    EXPECT_EQ("No World War 2", res.flag);
    EXPECT_EQ("enter", res.verb);
    EXPECT_EQ("does not", res.tags.size() ? res.tags[0] : std::string(""));
}

// ============= EventLoader Tests =============

// Test: Event struct basic construction
TEST(EventStructTest, EventStructHasCorrectFields) {
    Event evt;
    evt.name = "World War II Begins";
    evt.probability = 0.95;
    
    EXPECT_EQ("World War II Begins", evt.name);
    EXPECT_DOUBLE_EQ(0.95, evt.probability);
}

// Test: EventLoader loads valid CSV successfully
TEST(EventLoaderTest, LoadsValidCSVSuccessfully) {
    // Create a temporary test CSV file
    std::ofstream testFile("test_events.csv");
    testFile << "name,probability\n";
    testFile << "World War II Begins,0.95\n";
    testFile << "Atomic Bomb Developed,0.80\n";
    testFile << "Cold War Starts,0.75\n";
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_events.csv");
    
    EXPECT_EQ(3, events.size());
    EXPECT_TRUE(events.count("world war ii begins") > 0);  // Lowercase key
    EXPECT_TRUE(events.count("atomic bomb developed") > 0);
    EXPECT_TRUE(events.count("cold war starts") > 0);
    
    // Verify probabilities
    EXPECT_DOUBLE_EQ(0.95, events["world war ii begins"].probability);
    EXPECT_DOUBLE_EQ(0.80, events["atomic bomb developed"].probability);
    EXPECT_DOUBLE_EQ(0.75, events["cold war starts"].probability);
    
    // Clean up
    std::remove("test_events.csv");
}

// Test: EventLoader handles missing file gracefully
TEST(EventLoaderTest, HandlesMissingFileGracefully) {
    EventLoader loader;
    auto events = loader.loadFromCSV("nonexistent_file.csv");
    
    EXPECT_TRUE(events.empty());
    EXPECT_FALSE(loader.getLastError().empty());
    EXPECT_NE(std::string::npos, loader.getLastError().find("Failed to open file"));
}

// Test: EventLoader skips empty lines
TEST(EventLoaderTest, SkipsEmptyLines) {
    std::ofstream testFile("test_empty_lines.csv");
    testFile << "name,probability\n";
    testFile << "Event One,0.50\n";
    testFile << "\n";  // Empty line
    testFile << "Event Two,0.60\n";
    testFile << "   \n";  // Whitespace only line
    testFile << "Event Three,0.70\n";
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_empty_lines.csv");
    
    EXPECT_EQ(3, events.size());
    EXPECT_TRUE(events.count("event one") > 0);
    EXPECT_TRUE(events.count("event two") > 0);
    EXPECT_TRUE(events.count("event three") > 0);
    
    std::remove("test_empty_lines.csv");
}

// Test: EventLoader handles malformed lines
TEST(EventLoaderTest, HandlesMalformedLines) {
    std::ofstream testFile("test_malformed.csv");
    testFile << "name,probability\n";
    testFile << "Valid Event,0.80\n";
    testFile << "No Comma Here\n";  // Missing comma
    testFile << "Another Valid,0.90\n";
    testFile << "Bad Probability,not_a_number\n";  // Invalid probability
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_malformed.csv");
    
    // Should load only valid lines
    EXPECT_EQ(2, events.size());
    EXPECT_TRUE(events.count("valid event") > 0);
    EXPECT_TRUE(events.count("another valid") > 0);
    
    std::remove("test_malformed.csv");
}

// Test: EventLoader validates probability range
TEST(EventLoaderTest, ValidatesProbabilityRange) {
    std::ofstream testFile("test_probability_range.csv");
    testFile << "name,probability\n";
    testFile << "Valid Low,0.0\n";
    testFile << "Valid High,1.0\n";
    testFile << "Valid Middle,0.5\n";
    testFile << "Invalid Negative,-0.1\n";
    testFile << "Invalid Too High,1.5\n";
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_probability_range.csv");
    
    // Should only load probabilities in [0.0, 1.0] range
    EXPECT_EQ(3, events.size());
    EXPECT_TRUE(events.count("valid low") > 0);
    EXPECT_TRUE(events.count("valid high") > 0);
    EXPECT_TRUE(events.count("valid middle") > 0);
    EXPECT_FALSE(events.count("invalid negative") > 0);
    EXPECT_FALSE(events.count("invalid too high") > 0);
    
    std::remove("test_probability_range.csv");
}

// Test: EventLoader stores with lowercase keys for case-insensitive lookup
TEST(EventLoaderTest, StoresWithLowercaseKeys) {
    std::ofstream testFile("test_case.csv");
    testFile << "name,probability\n";
    testFile << "WORLD WAR II,0.95\n";
    testFile << "Cold War Starts,0.75\n";
    testFile << "moon landing,0.60\n";
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_case.csv");
    
    // All keys should be lowercase
    EXPECT_TRUE(events.count("world war ii") > 0);
    EXPECT_TRUE(events.count("cold war starts") > 0);
    EXPECT_TRUE(events.count("moon landing") > 0);
    
    // Original case should NOT work (keys are lowercase)
    EXPECT_FALSE(events.count("WORLD WAR II") > 0);
    EXPECT_FALSE(events.count("Cold War Starts") > 0);
    
    std::remove("test_case.csv");
}

// Test: EventLoader handles event names with commas
TEST(EventLoaderTest, HandlesCommasInEventNames) {
    std::ofstream testFile("test_commas.csv");
    testFile << "name,probability\n";
    testFile << "Event Name, With Comma,0.80\n";  // Comma in name (finds last comma)
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_commas.csv");
    
    // Should use last comma as separator
    EXPECT_EQ(1, events.size());
    // The name should be everything before the last comma
    EXPECT_TRUE(events.count("event name, with comma") > 0);
    
    std::remove("test_commas.csv");
}

// Test: EventLoader trims whitespace from fields
TEST(EventLoaderTest, TrimsWhitespace) {
    std::ofstream testFile("test_whitespace.csv");
    testFile << "name,probability\n";
    testFile << "  Event With Spaces  , 0.75 \n";
    testFile << "Another Event,0.85   \n";
    testFile.close();
    
    EventLoader loader;
    auto events = loader.loadFromCSV("test_whitespace.csv");
    
    EXPECT_EQ(2, events.size());
    EXPECT_TRUE(events.count("event with spaces") > 0);
    EXPECT_TRUE(events.count("another event") > 0);
    EXPECT_DOUBLE_EQ(0.75, events["event with spaces"].probability);
    EXPECT_DOUBLE_EQ(0.85, events["another event"].probability);
    
    std::remove("test_whitespace.csv");
}

// Test: Load actual events.csv file (integration test)
TEST(EventLoaderTest, LoadsActualEventsCSV) {
    EventLoader loader;
    
    // Try multiple possible paths
    auto events = loader.loadFromCSV("events.csv");
    if (events.empty()) {
        events = loader.loadFromCSV("../../data/events.csv");
    }
    if (events.empty()) {
        events = loader.loadFromCSV("../data/events.csv");
    }
    
    // Should load at least 5 events from the actual CSV
    EXPECT_GE(events.size(), 5);
    
    // Verify expected events exist
    EXPECT_TRUE(events.count("world war ii begins") > 0 || 
                events.count("atomic bomb developed") > 0);
}

// Test: EventLoader clears previous errors on successful load
TEST(EventLoaderTest, ClearsErrorOnSuccessfulLoad) {
    EventLoader loader;
    
    // First, try to load a non-existent file
    auto events1 = loader.loadFromCSV("nonexistent.csv");
    EXPECT_FALSE(loader.getLastError().empty());
    
    // Now load a valid file
    std::ofstream testFile("test_valid.csv");
    testFile << "name,probability\n";
    testFile << "Test Event,0.50\n";
    testFile.close();
    
    auto events2 = loader.loadFromCSV("test_valid.csv");
    EXPECT_EQ(1, events2.size());
    EXPECT_TRUE(loader.getLastError().empty());  // Error should be cleared
    
    std::remove("test_valid.csv");
}
