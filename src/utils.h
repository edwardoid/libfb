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

#include <cmath>
#include <limits>

struct Rect
{
    int32_t left = std::numeric_limits<int32_t>::max();
    int32_t top = std::numeric_limits<int32_t>::max();
    int32_t right = std::numeric_limits<int32_t>::min();
    int32_t bottom = std::numeric_limits<int32_t>::min();
    inline void includePoint(int32_t x, int32_t y)
    {
        left = std::min(x, left);
        right = std::max(right, x);
        top = std::min(top, y);
        bottom = std::max(bottom, y);
    }

    inline void includeRect(int32_t x, int32_t y, int32_t w, int32_t h)
    {
        includePoint(x, y);
        includePoint(x + w, y + h);
    }

    inline int32_t w() const { return right - left; }
    inline int32_t h() const { return bottom - top; }

    inline void moveTo(int32_t x, int32_t y)
    {
        int32_t w = Rect::w();
        int32_t h = Rect::h();
        left = x;
        top = y;
        right = x + w;
        bottom = y + h;
    }

    inline bool isValid() const
    {
        return left < right && bottom < top;
    }

    inline std::string toString()
    {
        std::stringstream ss;
        ss << "[(" << left << ", " << top << ") (" << right << ", " << bottom << ")] w = " << w() << " h = " << h();
        return ss.str();
    }
};