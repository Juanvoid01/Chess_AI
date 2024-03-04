#include "TranspositionTable.hpp"

TranspositionTable::TranspositionTable()
{
    const int MB = 64;
    int tableSize = MB * 1024 * 1024;
    int numEntries = tableSize / sizeof(TranspositionTable::Entry);

    entries.resize(numEntries);
    Clear();
    count = numEntries;

}

// get the stored value of the entry[zobristValue], returns -1 if there is no value stored
int TranspositionTable::GetEvaluation(uint64_t zobristValue, int depth, int plyFromRoot, int alpha, int beta)
{

    TranspositionTable::Entry entry = entries[GetIndex(zobristValue)];

    if (entry.key != zobristValue)
        return lookupFailed;

    // only valid if we are in the same or more depth of search
    if (entry.depth < depth)
        return lookupFailed;

    int score = entry.value;

    if (IsMateScore(score))
    {
        score = score < 0 ? -immediateMateScore : immediateMateScore;
    }

    if (entry.nodeType == exact)
    {
        return score;
    }
    else if (entry.nodeType == upperBound && score <= alpha)
    {
        // We have stored the upper bound of the eval for this position.
        return score;
    }
    else if (entry.nodeType == lowerBound && score >= beta)
    {
        // We have stored the lower bound of the eval for this position.
        return score;
    }

    return lookupFailed;
}

// inserts a new entry in the hash table
void TranspositionTable::StoreEvaluation(uint64_t zobristValue, int depth, int numPlySearched, int eval, int evalType, Move move)
{

    if (IsMateScore(eval))
    {
        eval = eval < 0 ? -immediateMateScore : immediateMateScore;
    }

    entries[GetIndex(zobristValue)] = Entry(zobristValue, eval, depth, evalType, move);
}
