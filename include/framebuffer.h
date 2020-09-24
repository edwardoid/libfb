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

#include <string>
#include <stdint.h>
#include <linux/fb.h>
#include "drawable.h"
#include "canvas.h"

BEGIN_LIBFB_NS
class FrameBuffer
{
public:
    FrameBuffer(std::string device = "");
    ~FrameBuffer();
    uint32_t bitsPerPixel() const;
    uint32_t bytesPerPixel() const;
    bool open(std::string device = "");
    bool close();
    void clear(uint32_t color = 0x000000);
    void flush();

    void drawPoint(uint32_t x, uint32_t y, uint32_t rgb);
    void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t rgb);
    void draw(const Drawable* pixmap);
    void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
    void drawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
    uint32_t width() const;
    uint32_t height() const;
    void refresh();

private:
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    protected:
        friend class FrameBuffer;
        uint8_t data[3];
    };

    Color convert(uint32_t rgb);
    Color convert(uint8_t* pos);

    uint32_t offset(uint32_t x, uint32_t y);
    void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Color& c);
    void drawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Color& c);
private:
    std::string m_device;
    int m_fbd = 0;
    struct fb_var_screeninfo m_varInfo;
    struct fb_fix_screeninfo m_fixInfo;
    uint8_t* m_mem;
    uint8_t* m_hw;
    uint32_t m_len;
};

END_LIBFB_NS
