#include "pch.h" // This is for precompiled headers, leave it in.

// The key step: Include the header from your main project.
// The ".." means "go up one directory level".
#include "../alternate-history-sim/SentenceProcessor.h"

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
/* 
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
*/