# Polyline Encoder
Another C++11 implementation of [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)

Usage examples:

```cpp
PolylineEncoder encoder;

// Poles and equator.
encoder.addPoint(-90, -180.0);
encoder.addPoint(.0, .0);
encoder.addPoint(90, 180.0);

auto res = encoder.encode(); // "~bidP~fsia@_cidP_gsia@_cidP_gsia@"
encoder.clear(); // Clear the list of points.
```

[![Build Status](https://travis-ci.org/vahancho/polylineencoder.svg?branch=master)][1]

Usage examples:

```cpp
PolylineEncoder encoder;

// Poles and equator.
encoder.addPoint(-90, -180.0);
encoder.addPoint(.0, .0);
encoder.addPoint(90, 180.0);

auto res = encoder.encode(); // "~bidP~fsia@_cidP_gsia@_cidP_gsia@"
encoder.clear(); // Clear the list of points.
```

[1]: https://travis-ci.org/vahancho/polylineencoder
