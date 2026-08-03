#include "header/simpleTracker.h"
#include "header/anyQueryTracker.h"
#include "header/outOfOrderTracker.h"

#include <cassert>
#include <iostream>

void SimpleTrackerTest() {
    SimpleTracker tracker(10);

    // An unknown symbol has no volume.
    assert(tracker.getVolume("AAPL", 100) == 0);

    // Trades are accumulated per symbol.
    tracker.addTrade({91, "AAPL", 100});
    tracker.addTrade({95, "AAPL", 50});
    tracker.addTrade({95, "MSFT", 200});
    assert(tracker.getVolume("AAPL", 100) == 150);
    assert(tracker.getVolume("MSFT", 100) == 200);

    // A trade at the cutoff is outside the window.
    assert(tracker.getVolume("AAPL", 101) == 50);

    // The remaining trade expires once it reaches the cutoff.
    assert(tracker.getVolume("AAPL", 105) == 0);

    // Expiring one symbol does not affect another symbol.
    assert(tracker.getVolume("MSFT", 105) == 0);

    std::cout << "All SimpleTracker tests passed.\n";
}

void AnyQueryTrackerTest() {
    AnyQueryTracker tracker;

    // An unknown symbol has no volume.
    assert(tracker.getVolume("AAPL", 10, 100) == 0);

    // Trades are added in timestamp order for each symbol.
    tracker.addTrade({91, "AAPL", 100});
    tracker.addTrade({95, "AAPL", 50});
    tracker.addTrade({95, "MSFT", 200});

    // The cutoff timestamp is excluded.
    assert(tracker.getVolume("AAPL", 10, 101) == 50);

    // Queries can be made in any order.
    assert(tracker.getVolume("AAPL", 10, 100) == 150);
    assert(tracker.getVolume("AAPL", 10, 105) == 0);
    assert(tracker.getVolume("MSFT", 10, 100) == 200);

    // Each query can use a different window size.
    assert(tracker.getVolume("AAPL", 6, 100) == 50);

    // Trades at the same timestamp are included in the prefix sum.
    tracker.addTrade({106, "AAPL", 25});
    tracker.addTrade({106, "AAPL", 15});
    assert(tracker.getVolume("AAPL", 10, 106) == 40);

    std::cout << "All AnyQueryTracker tests passed.\n";
}

void OutOfOrderTrackerTest() {
    OutOfOrderTracker tracker(5, 10);

    // Trades remain buffered until they are behind the watermark.
    tracker.addTrade({100, "AAPL", 100});
    assert(tracker.getVolume("AAPL") == 0);

    tracker.addTrade({103, "AAPL", 50});
    assert(tracker.getVolume("AAPL") == 0);

    // maxSeenTimestamp = 106, watermark = 101: timestamp 100 is confirmed.
    tracker.addTrade({106, "AAPL", 25});
    assert(tracker.getVolume("AAPL") == 100);

    // maxSeenTimestamp = 108, watermark = 103: timestamp 103 is confirmed.
    tracker.addTrade({108, "AAPL", 10});
    assert(tracker.getVolume("AAPL") == 150);

    // A trade at or before the watermark is too late and is ignored.
    tracker.addTrade({100, "AAPL", 999});
    assert(tracker.getVolume("AAPL") == 150);

    // Advancing the watermark confirms newer trades and expires old ones.
    tracker.addTrade({120, "AAPL", 5});
    assert(tracker.getVolume("AAPL") == 35);
    assert(tracker.getVolume("UNKNOWN") == 0);

    std::cout << "All OutOfOrderTracker tests passed.\n";
}

int main() {
    SimpleTrackerTest();
    AnyQueryTrackerTest();
    OutOfOrderTrackerTest();
    return 0;
}
