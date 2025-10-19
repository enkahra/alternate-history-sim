#include "FlagGenerator.h"


static std::string normalizeNegative(const std::string& neg)
{
    // Normalize common contractions/variants to a canonical form
    if (neg == "dont" || neg == "don't") return "do not";
    if (neg == "doesnt" || neg == "doesn't") return "does not";
    if (neg == "cant" || neg == "can't") return "cannot";
    if (neg == "wont" || neg == "won't") return "will not";
    return neg;
}

FlagGenerator::FlagGenerator()
{
    // Negative words: Expanded to ~25 entries, including single words, phrases, contractions, and synonyms
    // Covers possibilities like "does not", "never", "won't", "fails", etc.
    negativeWords =
    {
        "does not", "do not", "doesnt", "dont", "don't",  // Contractions and full forms
        "never", "not", "no",  // Single negatives
        "fails", "fails to", "failed", "failed to",  // Failure indicators
        "cannot", "cant", "can't",  // Inability
        "won't", "wont", "will not",  // Future negation
        "dies", "died", "collapses", "collapsed",  // Endings (e.g., empires)
        "avoids", "avoids to", "prevents", "prevents to",  // Prevention
        "lacks", "lacks to", "misses", "misses to",  // Absence
        "abandons", "abandons to", "gives up",  // Giving up
        "ignores", "ignores to", "overlooks"  // Neglect
    };

    // Event keywords: map phrases to specific flag names (prefer most specific labels)
    eventKeywords =
    {
        {"atom bomb", "Nuke"}, {"nuclear bomb", "Nuke"}, {"nuclear weapon", "Nuke"},  // Atomic themes
        // Prefer specific war names when available
        {"world war 2", "World War 2"}, {"world war ii", "World War 2"}, {"world war", "World War"}, {"war", "War"}, {"civil war", "War"},  // Conflicts
        {"ww2", "World War 2"}, {"wwii", "World War 2"}, // common abbreviations
        {"invention", "Tech"}, {"industrial revolution", "Tech"}, {"steam engine", "Tech"},  // Innovations
        {"empire", "Empire"}, {"ottoman empire", "Empire"}, {"roman empire", "Empire"},  // Empires
        {"collapse", "Collapse"}, {"fall", "Collapse"}, {"downfall", "Collapse"},  // Endings
        {"plague", "Plague"}, {"black death", "Plague"}, {"pandemic", "Plague"},  // Diseases
        {"revolution", "Revolution"}, {"french revolution", "Revolution"}, {"american revolution", "Revolution"},  // Uprisings
        {"discovery", "Discovery"}, {"new world", "Discovery"}, {"america discovery", "Discovery"},  // Explorations
        {"alliance", "Alliance"}, {"nato", "Alliance"}, {"axis powers", "Alliance"}  // Alliances
    };

    // verbs lexicon: now stored as a member `verbs` alongside negativeWords and eventKeywords
    verbs = {
        "enter", "entering", "entered", "invade", "invaded", "invading", "develop", "develops", "developed", "developing",
        "create", "creates", "created", "discover", "discovers", "discovered", "discovering", "collapse", "collapses", "collapsed",
        "join", "joins", "joined", "leave", "leaves", "left", "attack", "attacks", "attacked"
    };
}

FlagResult FlagGenerator::generateFlag(const std::vector<std::string>& actionWords)
{
    FlagResult result;
    std::string negativePhrase;
    std::string flag = "";

    // Prepare lowercase copy of action words for case-insensitive matching
    std::vector<std::string> lw;
    lw.reserve(actionWords.size());
    for (const auto& w : actionWords) {
        std::string s = w;
        for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        lw.push_back(s);
    }

    // detect verb/intent (also record index)
    int verbIndex = -1;
    for (size_t i = 0; i < lw.size(); ++i) {
        if (verbs.count(lw[i])) {
            result.verb = lw[i];
            verbIndex = static_cast<int>(i);
            break;
        }
    }

    // Negative scan (record index)
    int negativeIndex = -1;
    for (size_t i = 0; i < lw.size(); ++i)
    {
        // 2-word check first (prefer phrase)
        if (i + 1 < lw.size())
        {
            std::string phrase2 = lw[i] + " " + lw[i + 1];
            if (negativeWords.count(phrase2))
            {
                negativePhrase = normalizeNegative(phrase2);
                negativeIndex = static_cast<int>(i);
                break;
            }
        }
        // Single word check
        if (negativeWords.count(lw[i]))
        {
            negativePhrase = normalizeNegative(lw[i]);
            negativeIndex = static_cast<int>(i);
            // don't break immediately; prefer a 2-word phrase found earlier
        }
    }

    // Event scan: collect all matches with indices
    struct Candidate { int pos; std::string label; std::string phrase; };
    std::vector<Candidate> matches; // index -> flag name
    for (size_t i = 0; i < lw.size(); ++i)
    {
        // 3-word check
        if (i + 2 < lw.size())
        {
            std::string phrase3 = lw[i] + " " + lw[i + 1] + " " + lw[i + 2];
            if (eventKeywords.count(phrase3))
            {
                matches.push_back({ static_cast<int>(i), eventKeywords.at(phrase3), phrase3 });
                continue; // continue collecting other matches
            }
        }
        // 2-word check
        if (i + 1 < lw.size())
        {
            std::string phrase2 = lw[i] + " " + lw[i + 1];
            if (eventKeywords.count(phrase2))
            {
                matches.push_back({ static_cast<int>(i), eventKeywords.at(phrase2), phrase2 });
                continue;
            }
        }
        // 1-word check
        if (eventKeywords.count(lw[i]))
        {
            matches.push_back({ static_cast<int>(i), eventKeywords.at(lw[i]), lw[i] });
        }
    }

    // choose best match: prefer one closest to verbIndex, then to negativeIndex, else first
    int chosenIdx = -1;
    std::string chosenFlag;
    std::string chosenPhrase;
    if (!matches.empty()) {
        if (verbIndex >= 0) {
            int bestDist = INT_MAX;
            int bestPos = -1;
            for (size_t i = 0; i < matches.size(); ++i) {
                int pos = matches[i].pos;
                int dist = std::abs(pos - verbIndex);
                if (dist < bestDist || (dist == bestDist && pos > verbIndex)) {
                    bestDist = dist;
                    bestPos = static_cast<int>(i);
                }
            }
            chosenIdx = matches[bestPos].pos;
            chosenFlag = matches[bestPos].label;
            chosenPhrase = matches[bestPos].phrase;
            result.matchedEvent = chosenPhrase;
        }
        else if (negativeIndex >= 0) {
            int bestDist = INT_MAX;
            int bestPos = -1;
            for (size_t i = 0; i < matches.size(); ++i) {
                int pos = matches[i].pos;
                int dist = std::abs(pos - negativeIndex);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestPos = static_cast<int>(i);
                }
            }
            chosenIdx = matches[bestPos].pos;
            chosenFlag = matches[bestPos].label;
            chosenPhrase = matches[bestPos].phrase;
            result.matchedEvent = chosenPhrase;
        }
        else {
            // fallback: pick first match
            chosenIdx = matches[0].pos;
            chosenFlag = matches[0].label;
            chosenPhrase = matches[0].phrase;
            result.matchedEvent = chosenPhrase;
        }
    }

    if (!negativePhrase.empty()) result.tags.push_back(negativePhrase);

    // Only produce a concrete flag if a negative phrase is present and we have a chosen event
    if (!negativePhrase.empty() && !chosenFlag.empty())
    {
        result.flag = "No " + chosenFlag;
        result.confidence = 1.0; // simple confident match
        return result;
    }

    // If an event was matched but no negative phrase found, return an intermediate confidence
    if (!chosenFlag.empty() && negativePhrase.empty()) {
        result.confidence = 0.5; // tentative positive match, but no negation
        return result;
    }

    // Do not return tentative positive flags when no negative is found
    result.confidence = 0.0;
    return result;
}
