#pragma once

#include "trade.h"

#include <deque>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

class OutOfOrderTracker {
private:
    struct SymbolQueue {
        std::deque<Trade> trades;
        long long total = 0LL;
    };

    struct TradeComp {
        bool operator()(const Trade& lhs, const Trade& rhs) const {
            return lhs.timestamp > rhs.timestamp;
        }
    };

    long long m_allowedLatency;
    long long m_windowSize;
    long long m_maxSeenTimestamp = std::numeric_limits<long long>::min();
    std::priority_queue<Trade, std::vector<Trade>, TradeComp> m_buffer;
    std::unordered_map<std::string, SymbolQueue> m_queue;

    bool hasTrade() const;
    long long watermark() const;
    void confirmTrades();
    void removeExpiredTrades();

public:
    OutOfOrderTracker(long long allowedLatency, long long windowSize);

    void addTrade(const Trade& trade);

    long long getVolume(const std::string& symbol) const;
};
