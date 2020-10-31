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

#include <libfb_globals.h>
#include <math.h>
#include <vector>
BEGIN_LIBFB_NS

namespace helpers
{
    
inline double distance(delta_t x1, delta_t y1, delta_t x2, delta_t y2)
{
    double dx = pow( delta_t(x1) - delta_t(x2), 2);
    double dy = pow( delta_t(y1) - delta_t(y2), 2);
    return sqrt(dx + dy);
}

    static color_t avg(const std::vector<color_t>& colors);
};

END_LIBFB_NS