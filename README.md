# Polyline Encoder
Another C++11 implementation of [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)

[![Build Status](https://travis-ci.org/vahancho/polylineencoder.svg?branch=master)][1]

## Example:

```cpp
PolylineEncoder encoder;

// Poles and equator.
encoder.addPoint(-90.0, -180.0);
encoder.addPoint(.0, .0);
encoder.addPoint(90.0, 180.0);

auto res = encoder.encode(); // "~bidP~fsia@_cidP_gsia@_cidP_gsia@"
encoder.clear(); // Clear the list of points.

// Decode a string using static function.
auto polyline = PolylineEncoder::decode("~bidP~fsia@_cidP_gsia@_cidP_gsia@");
```

## See Also

* [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)

[1]: https://travis-ci.org/vahancho/polylineencoder
