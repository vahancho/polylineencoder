/**********************************************************************************
*  MIT License                                                                    *
*                                                                                 *
*  Copyright (c) 2017 Vahan Aghajanyan <vahancho@gmail.com>                       *
*                                                                                 *
*  Permission is hereby granted, free of charge, to any person obtaining a copy   *
*  of this software and associated documentation files (the "Software"), to deal  *
*  in the Software without restriction, including without limitation the rights   *
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
*  copies of the Software, and to permit persons to whom the Software is          *
*  furnished to do so, subject to the following conditions:                       *
*                                                                                 *
*  The above copyright notice and this permission notice shall be included in all *
*  copies or substantial portions of the Software.                                *
*                                                                                 *
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
*  SOFTWARE.                                                                      *
***********************************************************************************/

#include <tuple>
#include <cinttypes>
#include <assert.h>
#include <cmath>

#include "polylineencoder.h"

static const double s_presision   = 100000.0;
static const int    s_chunkSize   = 5;
static const int    s_asciiOffset = 63;
static const int    s_5bitMask    = 0x1f; // 0b11111 = 31
static const int    s_6bitMask    = 0x20; // 0b100000 = 32

PolylineEncoder::Point::Point(double latitude, double longitude)
    : m_latitude(std::round(latitude * s_presision) / s_presision)
    , m_longitude(std::round(longitude * s_presision) / s_presision)
{
    assert(latitude <= 90.0 && latitude >= -90.0);
    assert(longitude <= 180.0 && longitude >= -180.0);
}

double PolylineEncoder::Point::latitude() const
{
    return m_latitude;
}

double PolylineEncoder::Point::longitude() const
{
    return m_longitude;
}

void PolylineEncoder::addPoint(double latitude, double longitude)
{
    m_polyline.emplace_back(latitude, longitude);
}

std::string PolylineEncoder::encode() const
{
    return encode(m_polyline);
}

std::string PolylineEncoder::encode(double value)
{
    int32_t e5 = std::round(value * s_presision); // (2)

    e5 <<= 1;                                     // (4)

    if (value < 0) {
        e5 = ~e5;                                 // (5)
    }

    bool hasNextChunk = false;
    std::string result;

    // Split the value into 5-bit chunks and convert each of them to integer
    do {
        int32_t nextChunk = (e5 >> s_chunkSize); // (6), (7) - start from the left 5 bits.
        hasNextChunk = nextChunk > 0;

        int charVar = e5 & s_5bitMask;           // 5-bit mask (0b11111 == 31). Extract the left 5 bits.
        if (hasNextChunk) {
            charVar |= s_6bitMask;               // (8)
        }
        charVar += s_asciiOffset;                // (10)
        result += (char)charVar;                 // (11)

        e5 = nextChunk;
    } while (hasNextChunk);

    return result;
}

std::string PolylineEncoder::encode(const PolylineEncoder::Polyline &polyline)
{
    std::string result;

    // The first segment: offset from (.0, .0)
    double latPrev = .0;
    double lonPrev = .0;

    for (const auto &point : polyline)
    {
      const auto lat = point.latitude();
      const auto lon = point.longitude();

      // Offset from the previous point
      result.append(encode(lat - latPrev));
      result.append(encode(lon - lonPrev));

      latPrev = lat;
      lonPrev = lon;
    }

    return result;
}

double PolylineEncoder::decode(const std::string &coords, size_t &i)
{
    assert(i < coords.size());

    int32_t result = 0;
    int shift = 0;
    char c = 0;
    do {
        if (i < coords.size()) {
            c = coords.at(i++);
            c -= s_asciiOffset;      // (10)
            result |= (c & s_5bitMask) << shift;
            shift += s_chunkSize;    // (7)
        } else {
            return NAN;
        }
    } while (c >= s_6bitMask);

    if (result & 1) {
        result = ~result;        // (5)
    }
    result >>= 1;                // (4)

    // Convert to decimal value.
    return result / s_presision; // (2)
}

PolylineEncoder::Polyline PolylineEncoder::decode(const std::string &coords)
{
    PolylineEncoder::Polyline polyline;

    size_t i = 0;
    while (i < coords.size())
    {
        double lat = decode(coords, i);
        if (std::isnan(lat) || fabs(lat) > 90.0) {
            // Invalid latitude, implies invalid polyline string.
            polyline.clear();
            break;  // exit while
        }

        double lon = NAN;
        if (i < coords.size()) {
            lon = decode(coords, i);
        }
        if (std::isnan(lon) || fabs(lon) > 180.0) {
            // Invalid longitude, implies invalid polyline string.
            polyline.clear();
            break;  // exit while
        }

        if (!polyline.empty()) {
            const auto &prevPoint = polyline.back();
            lat += prevPoint.latitude();
            lon += prevPoint.longitude();
        }
        polyline.emplace_back(lat, lon);
    }

    return polyline;
}

const PolylineEncoder::Polyline &PolylineEncoder::polyline() const
{
    return m_polyline;
}

void PolylineEncoder::clear()
{
    m_polyline.clear();
}
