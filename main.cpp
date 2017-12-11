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

#include "../polylineencoder.cpp"

static bool test1()
{
    PolylineEncoder encoder;
    encoder.addPoint(.0, .0);
    
    auto res = encoder.encode();
    if (res == "??") {
        return true;
    }
    
    fprintf(stderr, "test1 fails\n");
    fprintf(stderr, "\tExpected: '%s', got: '%s'\n", "??", res.c_str());
    return false;    
}

static bool test2()
{
    PolylineEncoder encoder;

    // Poles and equator.
    encoder.addPoint(-90, -180.0);
    encoder.addPoint(.0, .0);
    encoder.addPoint(90, 180.0);
    
    auto res = encoder.encode();
    std::string exp("~bidP~fsia@_cidP_gsia@_cidP_gsia@");
    if (res == exp) {
        return true;
    }
    
    fprintf(stderr, "test2 fails\n");
    fprintf(stderr, "\tExpected: '%s', got: '%s'\n", exp.c_str(), res.c_str());
    return false;    
}

int main(int /*argc*/, char ** /*argv[]*/)
{
    printf("Start PolylineEncoder tests\n");

    if (!test1() ||
        !test2()) {
        return 1;
    }
    
    printf("PolylineEncoder tests passed\n");
    return 0;
}
