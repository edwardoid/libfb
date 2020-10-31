/************************************************************************
 * lifb - Simple library for drawing on Linux FrameBuffer
 *
 * Copyright (C) 2019  Eduard Sargsyan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#pragma once


#include <cstdint>
#include <stdint.h>

#ifndef LIBFB_NS
    #define LIBFB_NS libfb
#endif // LIBFB_NS

#define BEGIN_LIBFB_NS namespace LIBFB_NS {
#define END_LIBFB_NS }

BEGIN_LIBFB_NS

using color_t = std::uint32_t;

#define INVALID_COLOR ((color_t) -1)

using pos_t = std::int16_t;
using delta_t = std::int32_t;
using dimension_t = std::int16_t;

namespace helpers
{
    static inline color_t r(const color_t& c)
    {
        return (c >> 24) & 0xFF;
    }
    static inline color_t g(const color_t& c)
    {
        return (c >> 16) & 0xFF;
    }

    static color_t b(const color_t& c)
    {
        return (c >> 8) & 0xFF;
    }

    static color_t a(const color_t& c)
    {
        return c & 0xFF;
    }

    static color_t c(color_t r, color_t g, color_t b, color_t a)
    {
        return (r << 24) | (g << 16) | (b << 8) | a;
    }
}

END_LIBFB_NS
