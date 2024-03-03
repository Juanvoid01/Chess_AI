#pragma once
#include <vector>
#include "Zobrist.hpp"

// Store the evaluations of chess positions using the concepts of dynamic programming
class TranspositionTable
{
public:
    TranspositionTable();

    ~TranspositionTable() {}

    // clear all the entries of the hash map
    inline void Clear()
    {
        // entries.clear();
        for (int i = 0; i < entries.size(); i++)
            entries[i] = Entry();
    }

    // get the stored value of the entry[zobristValue], returns -1 if there is no value stored
    int GetEvaluation(uint64_t zobristValue, int depth, int plyFromRoot, int alpha, int beta);

    // inserts a new entry in the hash table
    void StoreEvaluation(uint64_t zobristValue, int depth, int numPlySearched, int eval, int evalType, Move move);

    // exact evaluation code 0
    const int exact = 0;

    // lower bound of evaluation code 1
    const int lowerBound = 1;

    // upper bound of evaluation code 2
    const int upperBound = 2;

    // error code -1
    const int lookupFailed = -1;

    // max value for a mate in 1
    const int immediateMateScore = 100000;

private:
    struct Entry;

    std::vector<Entry> entries;
    unsigned long count;

    // because the hashValue is too long, the real entry in the table is calculated with hashValue % count
    inline uint64_t GetIndex(uint64_t zobristHashValue) const { return zobristHashValue % count; }

    // mate in 1 has a score of 100000, longer mates have 100000 - ply, if score is too high, its a mate
    inline bool IsMateScore(int score) const { return abs(score) > 90000; }

    struct Entry
    {

        uint64_t key;
        int value;
        uint8_t depth;
        uint8_t nodeType;
        Move move;

    public:
        Entry(uint64_t key, int value, uint8_t depth, uint8_t nodeType, Move move)
            : key(key), value(value), depth(depth), nodeType(nodeType), move(move)
        {
        }

        Entry()
        {
        }
    };

public:
    inline Entry GetEntry(uint64_t zobristValue) const { return entries[GetIndex(zobristValue)]; }
};
