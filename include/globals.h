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

END_LIBFB_NS