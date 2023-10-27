# A C++ header-only Polyline Encoder/Decoder
C++ implementation of [Google Encoded Polyline Algorithm Format.](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)<br>
The implementation guarantees to conform with the results of the [Google Interactive Polyline Encoder Utility.](https://developers.google.com/maps/documentation/utilities/polylineutility)

[![Latest release](https://img.shields.io/github/v/release/vahancho/polylineencoder?include_prereleases)](https://github.com/vahancho/polylineencoder/releases)
[![Build and test (CMake)](https://github.com/vahancho/polylineencoder/actions/workflows/cmake.yml/badge.svg)](https://github.com/vahancho/polylineencoder/actions/workflows/cmake.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/6tg1kkp5fgk3x2fd?svg=true)](https://ci.appveyor.com/project/vahancho/polylineencoder)
[![Code coverage](https://codecov.io/gh/vahancho/polylineencoder/branch/master/graph/badge.svg)](https://codecov.io/gh/vahancho/polylineencoder)

## Prerequisites

No special requirements except *C++11* compliant compiler. The class is tested with
*gcc* and *MSVC* compilers. In order to build and run unit tests
for this project you are required to have Google Test library installed on the system.
For more details see the CI badges or [GitHub Actions](https://github.com/vahancho/polylineencoder/actions).

## Installation

As `Polylineencoder` is a single header library no installation required in general. Just include
*polylineencoder.h* in your project and instantiate `gepaf::PolylineEncoder` class template.

### Integration with `CMake` projects

However, if you use `CMake` and want to integrate the library into your project
you might want to install it first by invoking a `CMake` command from the build directory:

```
cmake --install . --prefix=<install_path>
```

Once the library is installed you can use it from in your project by adjusting its
`CMake` script. For example:

```
[..]
find_package(polylineencoder REQUIRED)

add_executable(example main.cpp)
target_link_libraries(example gepaf::polylineencoder)
[..]
```

## Usage Example:

All code is in `gepaf` namespace. `gepaf` stands for *Google Encoded Polyline Algorithm Format*.

```cpp
#include <polylineencoder.h>

// Create an encoder with precision of five decimal places (default)
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

You can also use custom data structures and containers including C-style arrays.

```cpp
class Point
{
public:
    Point(double t_x, double t_y) : x(t_x), y(t_y) {}

    double getX() const { return x; }
    double getY() const { return y; }

    double x;
    double y;
};

gepaf::PolylineEncoder<> encoder;

// List of DummyPoint for testing purpose
DummyPoint dp[] = {
    Point{-90.0, -180.0},
    Point{.0, .0},
    Point{90.0, 180.0}
};

// Using member functions.
auto result1 = encoder.encode(dp, dp + sizeof dp / sizeof dp[0], &DummyPoint::getX, &DummyPoint::getY);

// Using member variables.
auto result2 = encoder.encode(dp, dp + sizeof dp / sizeof dp[0], &DummyPoint::x, &DummyPoint::y);
```

## Building and Testing

There are unit tests provided for `PolylineEncoder` class template. You can find them in the *test/* directory.
To run them you have to build and run the test application (linking with Google
Test library is required). For doing that you can invoke the following commands
from the terminal, assuming that compiler and environment are already configured:

##### Linux (gcc)
```
cd test
g++ -std=c++11 -I..\src main.cpp -o test
./test
```

or with `CMake`

```
mkdir build && cd build
cmake .. -DENABLE_TESTING=True
make
make test
```

##### Windows
```
cd test
cl /W4 /EHsc /I..\src main.cpp /link /out:test.exe
test
```

or with CMake

```
mkdir build && cd build
cmake .. -G "NMake Makefiles" -DENABLE_TESTING=True
cmake --build . --config Release
ctest -C Release
```

## See Also

* [Encoded Polyline Algorithm](https://developers.google.com/maps/documentation/utilities/polylinealgorithm)
