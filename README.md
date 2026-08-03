# SimpleTracker

`SimpleTracker` tracks trading volume for each symbol over a rolling time window.

## Interface

The skeleton exposes two operations:

```cpp
void addTrade(const Trade& trade);
long long getVolume(const std::string& symbol, long long currentTime);
```

- `addTrade` records a trade.
- `getVolume` returns the volume for `symbol` at `currentTime`.

The constructor takes the window size:

```cpp
SimpleTracker tracker(windowSize);
```

For a window size of `10` and a query time of `100`, only trades with timestamps strictly greater than `90` are included. A trade at timestamp `90` is expired.

## Assumptions

This implementation is intentionally simple and makes the following assumptions:

- Trades are added in non-decreasing timestamp order for each symbol.
- Queries move forward in time. `getVolume` removes expired trades, so querying an earlier time after a later query cannot restore trades that were removed.
- `windowSize` is expected to be fixed.



## Build and run

With CMake:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Or compile directly:

```sh
c++ -std=c++17 main.cpp src/simpleTracker.cpp -o simple_tracker_tests
./simple_tracker_tests
```

`main.cpp` contains basic tests for multiple symbols, rolling-window expiry, unknown symbols, and the cutoff boundary.