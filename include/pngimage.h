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

#ifndef PNG_IMAGE_H
#define PNG_IMAGE_H

#include "pixmap.h"
#include <string>
#include <stdint.h>
#include <png.h>

namespace IoT
{
    class PNGImage: public Pixmap
    {
    public:
        PNGImage(std::string path = "");
        ~PNGImage();
        bool open(std::string path);
        void close();
        uint32_t rgba(uint32_t x, uint32_t y) const override;
        uint32_t width() const override;
        uint32_t height() const override;
    private:
        uint32_t m_width;
        uint32_t m_height;
        png_bytep *m_row_pointers;
    };
}


#endif // PNG_IMAGE_H