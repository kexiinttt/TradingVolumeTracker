#pragma once

#include "trade.h"

#include <string>
#include <unordered_map>
#include <vector>

class AnyQueryTracker {
private:
    struct SymbolHistory {
        std::vector<long long> timestamps;
        std::vector<long long> prefixSums;
    };

    std::unordered_map<std::string, SymbolHistory> m_history;

public:
    AnyQueryTracker() = default;

    void addTrade(const Trade& trade);

    long long getVolume(const std::string& symbol, long long windowSize, long long currentTime);
};
