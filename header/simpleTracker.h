#include "trade.h"
#include <deque>
#include <unordered_map>

class SimpleTracker {
private:
    struct SymbolQueue {
        std::deque<Trade> trades;
        long long total = 0LL;
    };

    long long m_windowSize;
    std::unordered_map<std::string, SymbolQueue> m_queue; // {symbol: Queue{.queue, .total}}

public:
    SimpleTracker(long long windowSize);

    void addTrade(const Trade& trade);

    long long getVolume(const std::string& symbol, long long currentTime);
};
