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

#include <tuple>
#include <cinttypes>
#include <assert.h>
#inlcude <cmath>

#include "polylineencoder.h"

void PolylineEncoder::addPoint(float latitude, float longitude)
{
    assert(latitude <= 90.0 && latitude >= -90.0);
    assert(latitude <= 180.0 && latitude >= -180.0);
    
    m_points.emplace_back(latitude, longitude);
}

std::string PolylineEncoder::encode() const
{
    std::string result;
    if (!m_points.empty()) {
        // The first segment: offset from (.0, .0)
        const auto &tuple0 = m_points.front();
        float latPrev = std::get<0>(tuple0);
        float lonPrev = std::get<1>(tuple0);

        result.append(encode(latPrev));
        result.append(encode(lonPrev));

        for (size_t i = 1; i < m_points.size(); i++) {
            const auto &tuple = m_points[i];
            const auto lat = std::get<0>(tuple);
            const auto lon = std::get<1>(tuple);

            // Offset from the previous point
            result.append(encode(lat - latPrev));
            result.append(encode(lon - lonPrev));

            latPrev = lat;
            lonPrev = lon;
        }
    }
    return result;
}

std::string PolylineEncoder::encode(float value) const
{
    int32_t e5 = std::round(value * 100000.0); // 2

    e5 <<= 1;                                  // (4)

    if (value < 0) {
        e5 = ~e5;                              // (5)
    }

    bool hasNextChunk = false;
    std::string result;

    // Split the value into 5-bit chunks and convert each of them to integer
    do
    {
        int32_t nextChunk = (e5 >> 5);         // (6), (7) - start from the left 5 bits.
        hasNextChunk = nextChunk > 0;

        int encVal = e5 & 0x1f; // 5-bit mask (0b11111 == 31). Extract the left 5 bits.
        if (hasNextChunk) {
            encVal |= 0x20;                    // (8)
        }
        encVal += 63;                          // (10)
        result += (char)encVal;                // (11)

        e5 = nextChunk;
    } while (hasNextChunk);

    return result;
}
