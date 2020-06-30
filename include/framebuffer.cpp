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

#include "framebuffer.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <iostream>

BEGIN_LIBFB_NS

FrameBuffer::Color FrameBuffer::convert(uint32_t rgb)
{
    Color c;
    c.r = (rgb >> 16) & 0xFF;
    c.g = (rgb >> 8) & 0xFF;
    c.b = (rgb >> 0) & 0xFF; 
    switch(m_varInfo.bits_per_pixel)
    {
        case 16: {
            uint8_t R5 = ( (int) c.r * 249 + 1014 ) >> 11;
            uint8_t G6 = ( (int) c.g * 253 +  505 ) >> 10;
            uint8_t B5 = ( (int) c.b * 249 + 1014 ) >> 11;
            uint16_t* px16 = (uint16_t*) (c.data);
            px16[0] = (R5 << 11) | (G6 << 5) | B5;
            break;
        }
        case 32:
        case 24: {
            c.data[0] = c.r;
            c.data[1] = c.g;
            c.data[2] = c.b;
            break;
        }
        default: {
            LOG_ERROR << "Unsupported color depth " << bitsPerPixel();
        }
    }

    return c;
}

FrameBuffer::Color FrameBuffer::convert(uint8_t* pos)
{
    Color c;
    switch(m_varInfo.bits_per_pixel)
    {
        case 16: {
            uint16_t* px16 = (uint16_t*) (c.data);
            *px16 = *((uint16_t*)pos);

            c.b = *px16 & 0x1F;
            c.g = (*px16 >> 5) & 0x3F;
            c.r = (*px16 >> 11) & 0x1F;

            c.r = ( (uint32_t)c.r * 527 + 23 ) >> 6;
            c.g = ( (uint32_t)c.g * 259 + 33 ) >> 6;
            c.b = ( (uint32_t)c.b * 527 + 23 ) >> 6;
            break;
        }
        case 32:
        case 24: {
            c.data[0] = pos[0];
            c.data[1] = pos[1];
            c.data[2] = pos[2];
            c.r = c.data[0];
            c.g = c.data[1];
            c.b = c.data[2];
            break;
        }
        default: {
            LOG_ERROR << "Unsupported color depth " << bitsPerPixel();
        }
    }
    return c;
}

uint32_t FrameBuffer::offset(uint32_t x, uint32_t y)
{
    if (x < m_varInfo.xres && y < m_varInfo.yres)
        return (x+m_varInfo.xoffset) * (m_varInfo.bits_per_pixel/8) + (y+m_varInfo.yoffset) * m_fixInfo.line_length;
    return -1;
}

FrameBuffer::FrameBuffer(std::string device)
    : m_device(device)
    , m_mem(nullptr)
{

}
FrameBuffer::~FrameBuffer() {
    close();
}

uint32_t FrameBuffer::width() const
{
    return m_varInfo.xres;
}

uint32_t FrameBuffer::height() const
{
    return m_varInfo.yres;
}

uint32_t FrameBuffer::bitsPerPixel() const
{
    return m_varInfo.bits_per_pixel;
}

uint32_t FrameBuffer::bytesPerPixel() const
{
    return m_varInfo.bits_per_pixel / 8;
}

bool FrameBuffer::open()
{
    m_fbd = ::open(m_device.c_str(), O_RDWR);
    if (m_fbd < 1) {
        m_fbd = 0;
        LOG_ERROR << "Can't open " << m_device << " error: " << strerror(errno);
        return false;
    }

    if(ioctl(m_fbd, FBIOGET_VSCREENINFO, &m_varInfo) == -1) {
		LOG_ERROR << "Error reading variable information. Error: " << strerror(errno);
		return false;
	}
	if(ioctl(m_fbd, FBIOGET_FSCREENINFO, &m_fixInfo) == -1) {
		LOG_ERROR << "Error reading fixed information. Error" << strerror(errno);
		return true;
	}

    m_hw = (uint8_t *) mmap(0, m_fixInfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbd, 0);
	m_hw += (m_varInfo.xoffset + m_varInfo.yoffset * m_varInfo.xres_virtual) * (m_varInfo.bits_per_pixel >> 3);
    m_len = m_fixInfo.smem_len - (m_varInfo.xoffset + m_varInfo.yoffset * m_varInfo.xres_virtual) * (m_varInfo.bits_per_pixel >> 3);
    m_mem = new uint8_t[m_fixInfo.smem_len];
    return true;
}

void FrameBuffer::clear(uint32_t color)
{
    if (color == 0)
        memset(m_mem, 0x0, m_fixInfo.smem_len);
    else {
        Color c = convert(color);
        uint32_t Bpp = bytesPerPixel();
        for(uint32_t i = 0; i < m_fixInfo.smem_len / Bpp; ++i) {
            memcpy(m_mem + i * Bpp, c.data, Bpp);
        }
    }
}

void FrameBuffer::drawPoint(uint32_t x, uint32_t y, uint32_t rgb)
{
    uint32_t pos = offset(x, y);
    if(pos == -1)
        return;
    Color c = convert(rgb);
    memcpy(m_mem + pos, c.data, bytesPerPixel());
}

void FrameBuffer::drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t rgb)
{
    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    int32_t D = 2*dy - dx;

    Color c = convert(rgb);
    uint32_t Bpp = bytesPerPixel();
    uint8_t r = rgb >> 16, g = (rgb >> 8) & 0xFF, b = rgb;
    for(int32_t x = x1, y = y1; x < x2; ++x) {
        uint32_t pos = offset(x, y);
        if (pos == -1)
            continue;
        memcpy(m_mem + pos, c.data, Bpp);
        if (D > 0) {
            ++y;
            D = D - 2*dx;
        }
        D = D + 2*dy;
    }
}

void FrameBuffer::flush() {
    if (m_mem != nullptr && memcmp(m_hw, m_mem, m_fixInfo.smem_len) != 0) {
        memcpy(m_hw, m_mem, m_fixInfo.smem_len);
    }
}

void FrameBuffer::refresh()
{
    //munmap(m_mem, m_fixInfo.smem_len);
    m_varInfo.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
	m_varInfo.yres_virtual = m_varInfo.yres * 2;
	m_varInfo.yoffset = m_varInfo.yres;
	::ioctl(m_fbd, FBIOGET_VSCREENINFO, &m_varInfo);
}

bool FrameBuffer::close()
{
    if (m_mem != nullptr) {
        delete []m_mem;
        m_mem = nullptr;
    }
    if (m_fbd == 0) {
        return true;
    }
    if(::close(m_fbd))
    {
        LOG_ERROR << "Can't close " << m_device << ". Error: " << strerror(errno);
        return false;
    }
    m_fbd = 0;
    return true;
}

void FrameBuffer::draw(const Drawable* d)
{
    pos_t x = d->x();
    pos_t y = d->y();
    uint32_t Bpp = bytesPerPixel();
    for (uint32_t i = 0; i < d->width(); ++i) {
        for (uint32_t j = 0; j < d->height(); ++j) {

            uint32_t pos = offset(x + i, y + j);
            uint32_t px = d->get(i, j);
            if (pos != -1 && px != INVALID_COLOR) {
                double alpha = (double)(px & 0xFF) / 255.;

                Color bg = convert(m_mem + pos);
                Color fg = convert(px >> 8);

                fg.r = alpha * (double) fg.r + (1. - alpha) * (double) bg.r;
                fg.g = alpha * (double) fg.g + (1. - alpha) * (double) bg.g;
                fg.b = alpha * (double) fg.b + (1. - alpha) * (double) bg.b;

                Color c = convert(fg.r << 16 | fg.g << 8 | fg.b);
                memcpy(m_mem + pos, c.data, Bpp);
            }
        }
    }
}

void FrameBuffer::fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    Color c = convert(color);
    fillRect(x, y, width, height, c);  
}

void FrameBuffer::drawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    Color c = convert(color);
    drawRect(x, y, width, height, c);  
}

void FrameBuffer::fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Color& c)
{
    uint32_t Bpp = bytesPerPixel();
    for(uint32_t i = 0; i < width; ++i) {
        for(uint32_t j = 0; j < height; ++j) {
            uint32_t pos = offset(x + i, y + j);    
            if (pos != -1) {
                memcpy(m_mem + pos, c.data, Bpp);
            }
        }
    }
}


void FrameBuffer::drawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Color& c)
{
    uint32_t Bpp = bytesPerPixel();
    for(uint32_t i = 0; i < width; ++i)
    {
        uint32_t posTop = offset(x + i, y);
        if (posTop != -1) {
                memcpy(m_mem + posTop, c.data, Bpp);   
        }
        uint32_t posBottom = offset(x + i, y + height);

        if (posBottom != -1) {
                memcpy(m_mem + posBottom, c.data, Bpp);   
        }
    }
    for(uint32_t j = 1; j < height - 1; ++j) {
        uint32_t posLeft = offset(x, y + j);
        if (posLeft != -1) {
            memcpy(m_mem + posLeft, c.data, Bpp);   
        }
        uint32_t posRight = offset(x + width, y + j);    
        if (posRight != -1) {
            memcpy(m_mem + posRight, c.data, Bpp);   
        }
    }
}

END_LIBFB_NS