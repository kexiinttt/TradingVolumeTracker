#include "../header/anyQueryTracker.h"

#include <algorithm>
#include <stdexcept>


void AnyQueryTracker::addTrade(const Trade& trade) {
    auto& history = m_history[trade.symbol];
    if (!history.timestamps.empty() && trade.timestamp == history.timestamps.back()) {
        history.prefixSums.back() += trade.quantity;
        return;
    }

    history.timestamps.push_back(trade.timestamp);
    const long long previousTotal = history.prefixSums.empty() ? 0LL : history.prefixSums.back();
    history.prefixSums.push_back(previousTotal + trade.quantity);
}

long long AnyQueryTracker::getVolume(const std::string& symbol, long long windowSize, long long currentTime) {
    auto it = m_history.find(symbol);
    if (it == m_history.end()) {
        return 0LL;
    }

    const auto& history = it->second;

    const long long cutoff = currentTime - windowSize;
    const auto first = std::upper_bound(history.timestamps.begin(), history.timestamps.end(), cutoff);
    const auto last = std::upper_bound(history.timestamps.begin(), history.timestamps.end(), currentTime);

    if (first == last) {
        return 0LL;
    }

    const auto firstIndex = static_cast<std::size_t>(first - history.timestamps.begin());
    const auto lastIndex = static_cast<std::size_t>(last - history.timestamps.begin());

    const long long beforeWindow = firstIndex == 0 ? 0LL : history.prefixSums[firstIndex - 1];
    return history.prefixSums[lastIndex - 1] - beforeWindow;
}
