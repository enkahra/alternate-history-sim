#include "pch.h" // This is for precompiled headers, leave it in.

// The key step: Include the header from your main project.
// The ".." means "go up one directory level".
#include "../alternate-history-sim/SentenceProcessor.h"
#include "../alternate-history-sim/Utils.h"

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
