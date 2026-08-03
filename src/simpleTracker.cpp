#include "../header/simpleTracker.h"

SimpleTracker::SimpleTracker(long long windowSize)
    : m_windowSize(windowSize) {}

void SimpleTracker::addTrade(const Trade& trade) {
    auto& symbolQueue = m_queue[trade.symbol];
    symbolQueue.trades.push_back(trade);
    symbolQueue.total += trade.quantity;
}

long long SimpleTracker::getVolume(const std::string& symbol, long long currentTime) {
    auto it = m_queue.find(symbol);
    if (it == m_queue.end()) {
        return 0;
    }

    auto& symbolQueue = it->second;
    const long long cutoff = currentTime - m_windowSize;

    while (!symbolQueue.trades.empty() && symbolQueue.trades.front().timestamp <= cutoff) {
        symbolQueue.total -= symbolQueue.trades.front().quantity;
        symbolQueue.trades.pop_front();
    }

    return symbolQueue.total;
}
