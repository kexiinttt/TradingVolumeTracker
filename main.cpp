#include "header/simpleTracker.h"

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

int main() {
    SimpleTrackerTest();
    return 0;
}
