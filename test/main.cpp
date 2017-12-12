/*
MIT License

Copyright (c) 2017 Vahan Aghajanyan <vahancho@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "../src/polylineencoder.cpp"

static bool test(const std::string &testName,
                 const PolylineEncoder &encoder,
                 const std::string &expected)
{
    auto res = encoder.encode();
    if (res == expected) {
        return true;
    }

    fprintf(stderr, "%s fails\n", testName.c_str());
    fprintf(stderr, "\tExpected: '%s', got: '%s'\n", expected.c_str(), res.c_str());
    return false;    
}

static bool test1()
{
    PolylineEncoder encoder;
    encoder.addPoint(.0, .0);

    return test("test1", encoder, "??");
}

static bool test2()
{
    PolylineEncoder encoder;

    // Poles and equator.
    encoder.addPoint(-90.0, -180.0);
    encoder.addPoint(.0, .0);
    encoder.addPoint(90.0, 180.0);

    return test("test2", encoder, "~bidP~fsia@_cidP_gsia@_cidP_gsia@");
}

static bool test3()
{
    // Empty list of points.
    PolylineEncoder encoder;

    return test("test3", encoder, std::string());
}

static bool test4()
{
    // Coordinates from https://developers.google.com/maps/documentation/utilities/polylinealgorithm
    PolylineEncoder encoder;
    encoder.addPoint(38.5, -120.2);
    encoder.addPoint(40.7, -120.95);
    encoder.addPoint(43.252, -126.453);

    return test("test4", encoder, "_p~iF~ps|U_ulLnnqC_mqNvxq`@");
}

int main(int /*argc*/, char ** /*argv[]*/)
{
    printf("Start PolylineEncoder tests\n");

    if (!test1() ||
        !test2() ||
        !test3() ||
        !test4()) {
        return 1;
    }
    
    printf("PolylineEncoder tests passed\n");
    return 0;
}
