#include "../header/outOfOrderTracker.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

OutOfOrderTracker::OutOfOrderTracker(long long allowedLatency, long long windowSize)
: m_allowedLatency(allowedLatency), m_windowSize(windowSize) {
    if (allowedLatency < 0 || windowSize < 0) {
        throw std::runtime_error("Cannot init instance with negative parameters");
    }    
}

bool OutOfOrderTracker::hasTrade() const {
    return m_maxSeenTimestamp != std::numeric_limits<long long>::min();
}

long long OutOfOrderTracker::watermark() const {
    return m_maxSeenTimestamp - m_allowedLatency;
}

void OutOfOrderTracker::confirmTrades() {
    const long long currentWatermark = watermark();

    while (!m_buffer.empty() && m_buffer.top().timestamp <= currentWatermark) {
        const Trade trade = m_buffer.top();
        m_buffer.pop();

        auto& symbolQueue = m_queue[trade.symbol];
        symbolQueue.trades.push_back(trade);
        symbolQueue.total += trade.quantity;
    }
}

void OutOfOrderTracker::removeExpiredTrades() {
    const long long cutoff = watermark() - m_windowSize;

    for (auto& [symbol, symbolQueue] : m_queue) {
        while (!symbolQueue.trades.empty() && symbolQueue.trades.front().timestamp <= cutoff) {
            symbolQueue.total -= symbolQueue.trades.front().quantity;
            symbolQueue.trades.pop_front();
        }
    }
}

void OutOfOrderTracker::addTrade(const Trade& trade) {
    if (hasTrade() && trade.timestamp <= watermark()) {
        std::cout << "Trade arrived later than allowed lateness" << std::endl;
        return;
    }

    m_maxSeenTimestamp = std::max(m_maxSeenTimestamp, trade.timestamp);
    m_buffer.push(trade);

    confirmTrades();
    removeExpiredTrades();
}

long long OutOfOrderTracker::getVolume(const std::string& symbol) const {
    auto it = m_queue.find(symbol);
    if (it == m_queue.end()) {
        return 0;
    }

    return it->second.total;
}
