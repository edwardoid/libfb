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

#include "helpers.h"

BEGIN_LIBFB_NS

color_t helpers::avg(const std::vector<color_t>& colors)
{
    color_t _r = 0, _g = 0, _b = 0, _a = 0;
    for(auto & _c : colors)
    {
        _r += r(_c);
        _g += g(_c);
        _b += b(_c);
        _a += a(_c);
    }

    return c(_r / colors.size(),
             _g / colors.size(),
             _b / colors.size(),
             _a / colors.size());
}

END_LIBFB_NS