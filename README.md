# A C++ header-only Polyline Encoder/Decoder
C++ implementation of [Google Encoded Polyline Algorithm Format.](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)<br>
The implementation guarantees to conform with the results of the [Google Interactive Polyline Encoder Utility.](https://developers.google.com/maps/documentation/utilities/polylineutility)

[![Latest release](https://img.shields.io/github/v/release/vahancho/polylineencoder?include_prereleases)](https://github.com/vahancho/polylineencoder/releases)
[![Build Status](https://travis-ci.org/vahancho/polylineencoder.svg?branch=master)](https://travis-ci.org/vahancho/polylineencoder)
[![Build status](https://ci.appveyor.com/api/projects/status/6tg1kkp5fgk3x2fd?svg=true)](https://ci.appveyor.com/project/vahancho/polylineencoder)
[![codecov](https://codecov.io/gh/vahancho/polylineencoder/branch/master/graph/badge.svg)](https://codecov.io/gh/vahancho/polylineencoder)

## Installation

As `Polylineencoder` is a single header library no installation required. Just include *polylineencoder.h* in your project and instantiate `gepaf::PolylineEncoder` class template.

## Prerequisites

No special requirements except *C++11* compliant compiler. The class is tested with *gcc 4.8.4* and *MSVC 12.0* (Visual Studio 2013). In order to build and run unit tests for this project you are required to have Google Test library installed on the system.
For more details see the CI badges (*Travis CI & AppVeyor CI*).

## Usage Example:

All code is in `gepaf` namespace. `gepaf` stands for *Google Encoded Polyline Algorithm Format*.

```cpp
#include <polylineencoder.h>

// Create an encoder with precision of 5 decimal places (default)
// In order to create objects with other precision use template parameter
// like: gepaf::PolylineEncoder<6>
gepaf::PolylineEncoder<> encoder;

// Poles and equator.
encoder.addPoint(-90.0, -180.0);
encoder.addPoint(.0, .0);
encoder.addPoint(90.0, 180.0);

auto res = encoder.encode(); // "~bidP~fsia@_cidP_gsia@_cidP_gsia@"
encoder.clear(); // Clear the list of points.

// Decode a string using static function.
auto polyline = gepaf::PolylineEncoder<>::decode("~bidP~fsia@_cidP_gsia@_cidP_gsia@");

// Iterate over all points and print coordinates of each.
for (const auto &point : polyline) {
    printf("(%f, %f)\n", point.latitude(), point.longitude());
}
```

## Building and Testing

There are unit tests provided for `PolylineEncoder` class template. You can find them in the *test/* directory.
To run them you have to build and run the test application (linking with Google Test library is required). For doing that you can invoke the following commands from the terminal, assuming that compiler and environment are already configured:

##### Linux (gcc)
```
cd test
g++ -std=c++11 -I..\src main.cpp -o test
./test
```

or with CMake

```
cd test
cmake ..
make
./poly_test
```

##### Windows
```
cd test
cl /W4 /EHsc /I..\src main.cpp /link /out:test.exe
test
```

or with CMake

```
cd test
cmake .. -G "NMake Makefiles"
nmake
poly_test
```

## See Also

* [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)

