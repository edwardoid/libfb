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

#include "drawable.h"
#include <string>
#include <png.h>

BEGIN_LIBFB_NS
class PNGImage: public Drawable
{
public:
    PNGImage(std::string path = "");
    ~PNGImage();
    bool open(std::string path);
    void close();
    color_t get(pos_t x, pos_t y) const override;
    dimension_t width() const override;
    dimension_t height() const override;
private:
    dimension_t m_width;
    dimension_t m_height;
    png_bytep *m_row_pointers;
};

END_LIBFB_NS
