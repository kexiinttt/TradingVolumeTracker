# Trading Volume Tracker

## 1. Simple Tracker

`SimpleTracker` tracks trading volume for each symbol over a rolling time window. This is the simple implementation based on the interviewer-provided skeleton.

### Interface

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

### Assumptions

- Trades are currently expected to be added in non-decreasing timestamp order for each symbol.
- Queries are currently expected to move forward in time.
- The tracker is intended for a fixed window size after construction.



## 2. Querying Any Timestamp and Any Window Size

The next version should allow `getVolume` to be called for any timestamp with any window size, regardless of the order in which queries are made.

### Assumptions

- Trades are currently expected to be added in non-decreasing timestamp order for each symbol.



## 3. Out-of-Order Trades

`OutOfOrderTracker` handles trades that arrive slightly later than trades with larger timestamps. It is constructed with an allowed latency and a fixed window size:

```cpp
OutOfOrderTracker tracker(allowedLatency, windowSize);
tracker.addTrade(trade);
long long volume = tracker.getVolume("AAPL");
```

The tracker maintains a watermark:

```text
watermark = largest timestamp seen - allowed latency
```

Trades at or before the watermark are confirmed from the min-heap buffer into the per-symbol queues. Once confirmed, trades older than `watermark - windowSize` are removed. A trade that arrives at or before the current watermark is considered too late and is ignored.

### Assumptions

- Trades arriving later than the allowed latency are discarded.
- `getVolume` returns the volume of confirmed, non-expired trades at the current watermark; it does not accept a query timestamp.

## Build and run

With CMake:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Or compile directly:

```sh
c++ -std=c++17 main.cpp src/simpleTracker.cpp src/anyQueryTracker.cpp src/outOfOrderTracker.cpp -o simple_tracker_tests
./simple_tracker_tests
```

`main.cpp` contains tests for all three tracker implementations, including
rolling-window expiry, arbitrary queries, watermark confirmation, and late
trade rejection.