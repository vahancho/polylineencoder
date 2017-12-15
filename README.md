# Polyline Encoder/Decoder
Another C++11 implementation of [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)

[![Build Status](https://travis-ci.org/vahancho/polylineencoder.svg?branch=master)](https://travis-ci.org/vahancho/polylineencoder)
[![Build status](https://ci.appveyor.com/api/projects/status/6tg1kkp5fgk3x2fd?svg=true)](https://ci.appveyor.com/project/vahancho/polylineencoder)

## Installation

No installation required. Just compile *polylineencoder.h(.cpp)* in your project and use `PolylineEncoder` class.

## Prerequisites

No special requirements except *C++11* compliant compiler. The class is tested with *gcc 4.8.4* and *MS C++ 12.0* (Visual Studio 2013).
For more details see the CI badges (*Travis CI & AppVeyor CI*).

## Usage Example:

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

// Iterate over all points and print coordinates of each.
for (const auto &point : polyline) {
    printf("(%f, %f)\n", std::get<0>(point), std::get<1>(point));
}
```

## Test

There are unit tests provided for `PolylineEncoder` class. You can find them in the *test/* directory.
To run them you have to build and run the test application. For doing that you must invoke the following
commands from the terminal, assuming that compiler and environment are already configured:

##### Linux (gcc)
```
cd test
g++ -std=c++11 test/main.cpp -o test
./test
```

##### Windows
```
cd test
cl /W4 /EHsc main.cpp /link /out:test.exe
test
```

## See Also

* [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)

