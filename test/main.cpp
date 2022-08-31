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

#include "polylineencoder.h"

#include <gtest/gtest.h>

template<typename Point>
bool operator==(const Point &l, const Point &r)
{
    EXPECT_DOUBLE_EQ(l.longitude(), r.longitude());
    EXPECT_DOUBLE_EQ(l.latitude(),  r.latitude());
    return true;
}

TEST(General, ZeroPoint)
{
    gepaf::PolylineEncoder<> encoder;
    encoder.addPoint(.0, .0);
    EXPECT_EQ(encoder.encode(), "??");
}

TEST(General, PolesAndEquator)
{
    gepaf::PolylineEncoder<> encoder;

    // Poles and equator.
    encoder.addPoint(-90.0, -180.0);
    encoder.addPoint(.0, .0);
    encoder.addPoint(90.0, 180.0);
    EXPECT_EQ(encoder.encode(), "~bidP~fsia@_cidP_gsia@_cidP_gsia@");
}

TEST(General, EmptyList)
{
    // Empty list of points.
    gepaf::PolylineEncoder<> encoder;

    EXPECT_EQ(encoder.encode(), std::string());
}

TEST(General, StandardExample)
{
    // Coordinates from https://developers.google.com/maps/documentation/utilities/polylinealgorithm
    gepaf::PolylineEncoder<> encoder;
    encoder.addPoint(38.5, -120.2);
    encoder.addPoint(40.7, -120.95);
    encoder.addPoint(43.252, -126.453);
    EXPECT_EQ(encoder.encode(), "_p~iF~ps|U_ulLnnqC_mqNvxq`@");

    const auto &polyline = encoder.polyline();
    // Expect three points.
    EXPECT_EQ(polyline.size(), 3);

    encoder.clear();
    EXPECT_EQ(polyline.size(), 0);
}

TEST(General, BasicDecode)
{
    // Decode a valid polyline string.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p~iF~ps|U_ulLnnqC_mqNvxq`@");
    EXPECT_EQ(decodedPolyline.size(), 3);
    EXPECT_TRUE(decodedPolyline[0] == gepaf::PolylineEncoder<>::Point(38.5, -120.2));
    EXPECT_TRUE(decodedPolyline[1] == gepaf::PolylineEncoder<>::Point(40.7, -120.95));
    EXPECT_TRUE(decodedPolyline[2] == gepaf::PolylineEncoder<>::Point(43.252, -126.453));

    decodedPolyline.clear();
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, HighPrecision_6_digits)
{
    {
        auto decodedPolyline = gepaf::PolylineEncoder<6>::decode("AA@@");
        EXPECT_EQ(decodedPolyline.size(), 2);
        EXPECT_TRUE(decodedPolyline[0] == gepaf::PolylineEncoder<6>::Point(0.0000005, 0.0000005));
        EXPECT_TRUE(decodedPolyline[1] == gepaf::PolylineEncoder<6>::Point(0.0, 0.0));
    }
    {
        gepaf::PolylineEncoder<6> encoder;
        encoder.addPoint(0.0000005, 0.0000005);
        encoder.addPoint(0.0000000, 0.0000000);
        EXPECT_EQ(encoder.encode(), "AA@@");
    }
    {
        gepaf::PolylineEncoder<6> encoder;
        encoder.addPoint(47.231174468994141, 16.62629508972168);
        encoder.addPoint(47.231208801269531, 16.626440048217773);
        EXPECT_EQ(encoder.encode(), "kkwayAmfxu^eAaH");

        auto decodedPolyline = gepaf::PolylineEncoder<6>::decode("kkwayAmfxu^eAaH");
        EXPECT_EQ(decodedPolyline.size(), 2);
        EXPECT_TRUE(decodedPolyline[0] == gepaf::PolylineEncoder<6>::Point(47.231174, 16.626295));
        EXPECT_TRUE(decodedPolyline[1] == gepaf::PolylineEncoder<6>::Point(47.231209, 16.626440));
    }
}

TEST(General, LowPrecision_1_digit)
{
    {
        gepaf::PolylineEncoder<1> encoder;
        encoder.addPoint(47.231174468994141, 16.54629508972168);
        encoder.addPoint(47.335208801269531, 16.65440048217773);
        EXPECT_EQ(encoder.encode(), "o\\iIAC");

        auto decodedPolyline = gepaf::PolylineEncoder<1>::decode("o\\iIAC");
        EXPECT_EQ(decodedPolyline.size(), 2);
        EXPECT_TRUE(decodedPolyline[0] == gepaf::PolylineEncoder<1>::Point(47.2, 16.5));
        EXPECT_TRUE(decodedPolyline[1] == gepaf::PolylineEncoder<1>::Point(47.3, 16.65));
    }
}

TEST(General, InvalidInputString1)
{
    // String too short, last byte missing makes last coordinate invalid.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p~iF~ps|U_ulLnnqC_mqNvxq`");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, InvalidInputString2)
{
    // String too short, last bytes missing makes last coordinate.lon missing.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p~iF~ps|U_ulLnnqC_mqN");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, InvalidInputString3)
{
    // String too short, last coordinate.lon missing and last coordinate.lat invalid.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p~iF~ps|U_ulLnnqC_mq");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, InvalidInputString4)
{
    // Third byte changed from '~' to ' ', generating an invalid fourth coordinate.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p iF~ps|U_ulLnnqC_mqNvxq`@");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, InvalidInputString5)
{
    // Fifth byte changed from 'F' to 'f' changing the 'next byte' flag in it,
    // leading to an extremely large latitude for the first coordinate.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p~if~ps|U_ulLnnqC_mqNvxq`@");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, InvalidInputString6)
{
    // Tenth byte changed from 'U' to 'u' changing the 'next byte' flag in it,
    // leading to an extremely large longitude for the first coordinate.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("_p~iF~ps|u_ulLnnqC_mqNvxq`@");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, DecodeEmptyString)
{
    // Empty string.
    auto decodedPolyline = gepaf::PolylineEncoder<>::decode("");
    EXPECT_EQ(decodedPolyline.size(), 0);
}

TEST(General, PrecisionTest)
{
    {
        // Avoid cumulated error
        gepaf::PolylineEncoder<> encoder;
        encoder.addPoint(0.0000005, 0.0000005);
        encoder.addPoint(0.0000000, 0.0000000);

        // Expectation comes from https://developers.google.com/maps/documentation/utilities/polylineutility
        EXPECT_EQ(encoder.encode(), "????");
    }
    {
        // Avoid cumulated error
        gepaf::PolylineEncoder<6> encoder;
        encoder.addPoint(0.00000005, 0.00000005);
        encoder.addPoint(0.00000000, 0.00000000);

        // Should be the same as with 5 digits precision.
        EXPECT_EQ(encoder.encode(), "????");
    }
}

TEST(General, PrecisionTest2)
{
    // Avoid cumulated error
    gepaf::PolylineEncoder<> encoder;
    encoder.addPoint(47.231174468994141, 16.62629508972168);
    encoder.addPoint(47.231208801269531, 16.626440048217773);

    // Expectation comes from https://developers.google.com/maps/documentation/utilities/polylineutility
    EXPECT_EQ(encoder.encode(), "yyg_HkindBG[");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

