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
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
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

using namespace IoT;

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
    if (m_mem != nullptr) {
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

void FrameBuffer::textSize(std::string font, uint32_t fontsize, std::string text, double angle, uint32_t& width, uint32_t& height)
{
    width = height = 0;
    FT_Library library;
    FT_Face face;
    FT_Matrix matrix; // transformation matrix
    FT_Vector pen;

    FT_UInt glyph_index;
    FT_Error error;

    FT_Bool use_kerning;
    FT_UInt previous = 0;

    /* Set up transformation Matrix */
    matrix.xx = (FT_Fixed)(cos(angle) * 0x10000);  /* It would make more sense to do this (below), but, bizzarely, */
    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000); /* if one does, FT_Load_Glyph fails consistently.               */
    matrix.yx = (FT_Fixed)(sin(angle) * 0x10000);  //   matrix.yx = - matrix.xy;
    matrix.yy = (FT_Fixed)(cos(angle) * 0x10000);  //   matrix.yy = matrix.xx;

  /* Place starting coordinates in adequate form. */
    pen.x = 0;
    pen.y = 0;

    int num_bytes = 0;
    while (text[num_bytes] != 0) {
       num_bytes++;
    }


    long *ucs4text;
    ucs4text = new long[num_bytes + 1];

    unsigned char u, v, w, x, y;

    int num_chars = 0;

    long iii = 0;

    while (iii < num_bytes) {
        unsigned char const z = text[iii];

        if (z <= 127) {
            ucs4text[num_chars] = z;
        }

        if ((192 <= z) && (z <= 223)) {
            iii++;
            y = text[iii];
            ucs4text[num_chars] = (z - 192) * 64 + (y - 128);
        }

        if ((224 <= z) && (z <= 239)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            ucs4text[num_chars] = (z - 224) * 4096 + (y - 128) * 64 + (x - 128);
        }

        if ((240 <= z) && (z <= 247)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            iii++;
            w = text[iii];
            ucs4text[num_chars] = (z - 240) * 262144 + (y - 128) * 4096 + (x - 128) * 64 + (w - 128);
        }

        if ((248 <= z) && (z <= 251)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            iii++;
            w = text[iii];
            iii++;
            v = text[iii];
            ucs4text[num_chars] = (z - 248) * 16777216 + (y - 128) * 262144 + (x - 128) * 4096 + (w - 128) * 64 + (v - 128);
        }

        if ((252 == z) || (z == 253)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            iii++;
            w = text[iii];
            iii++;
            v = text[iii];
            u = text[iii];
            ucs4text[num_chars] = (z - 252) * 1073741824 + (y - 128) * 16777216 + (x - 128) * 262144 + (w - 128) * 4096 + (v - 128) * 64 + (u - 128);
        }

        if ((z == 254) || (z == 255))
        {
            LOG_WARNING << "Problem with character: invalid UTF-8 data." << std::endl;
            return;
        }
        iii++;
        num_chars++;
    }

    // num_chars now contains the number of characters in the string.
    /* Initialize FT Library object */
    error = FT_Init_FreeType(&library);
    if (error) {
        LOG_WARNING << "FreeType: Could not init Library." << std::endl;
        delete[] ucs4text;
        return;
    }

    /* Initialize FT face object */
    error = FT_New_Face(library, font.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        LOG_WARNING << "FreeType: Font was opened, but type not supported." << std::endl;
        delete[] ucs4text;
        return;
    }
    else if (error)
    {
        LOG_WARNING << "FreeType: Could not find or load font file." << std::endl;
        delete[] ucs4text;
        return;
    }

    /* Set the Char size */
    error = FT_Set_Char_Size(face,          /* handle to face object           */
                             0,             /* char_width in 1/64th of points  */
                             fontsize * 64, /* char_height in 1/64th of points */
                             100,           /* horizontal device resolution    */
                             100);          /* vertical device resolution      */

    /* A way of accesing the glyph directly */
    FT_GlyphSlot slot = face->glyph; // a small shortcut

    /* Does the font file support kerning? */
    use_kerning = FT_HAS_KERNING(face);

    uint32_t estimatedWidth = 0;

    for (int n = 0; n < num_chars; n++) {
        /* Convert character code to glyph index */
        glyph_index = FT_Get_Char_Index(face, ucs4text[n]);

        /* Retrieve kerning distance and move pen position */
        if (use_kerning && previous && glyph_index) {
            FT_Vector delta;
            FT_Get_Kerning(face,
                           previous,
                           glyph_index,
                           ft_kerning_default, //FT_KERNING_DEFAULT,
                          &delta);

            /* Transform this kerning distance into rotated space */
            pen.x += (int)(((double)delta.x) * cos(angle));
            pen.y += (int)(((double)delta.x) * (sin(angle)));
        }

        /* Set transform */
        FT_Set_Transform(face, &matrix, &pen);

        if (error) {
            LOG_WARNING << "FreeType: Set char size error.";
            delete[] ucs4text;
            return;
        }

        /* Retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index(face, ucs4text[n]);

        /* Load glyph image into the slot (erase previous one) */
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error) {
            LOG_WARNING << "FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error << ").";
            delete[] ucs4text;
            return;
        }

        /* Convert to an anti-aliased bitmap */
        error = FT_Render_Glyph(face->glyph, ft_render_mode_normal);
        if (error) {
            LOG_WARNING << "FreeType: Render glyph error.";
            delete[] ucs4text;
            return;
        }

        if (n == 0 || height < (fontsize - slot->bitmap_top + slot->bitmap.rows)) {
            height = fontsize - slot->bitmap_top + slot->bitmap.rows;
        }
        if (n = 0 || width < (slot->bitmap_left + slot->bitmap.width)) {
            width = slot->bitmap_left + slot->bitmap.width;
        }

        /* Advance to the next position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;

        /* record current glyph index */
        previous = glyph_index;
    }

    /* Free the face and the library objects */
    FT_Done_Face(face);
    FT_Done_FreeType(library);

    delete[] ucs4text;
}

bool FrameBuffer::drawText(std::string font, uint32_t fontsize, uint32_t x_start, uint32_t y_start, double angle, std::string text, uint32_t color)
{
    FT_Library library;
    FT_Face face;
    FT_Matrix matrix; // transformation matrix
    FT_Vector pen;

    FT_UInt glyph_index;
    FT_Error error;

    FT_Bool use_kerning;
    FT_UInt previous = 0;

    /* Set up transformation Matrix */
    matrix.xx = (FT_Fixed)(cos(angle) * 0x10000);  /* It would make more sense to do this (below), but, bizzarely, */
    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000); /* if one does, FT_Load_Glyph fails consistently.               */
    matrix.yx = (FT_Fixed)(sin(angle) * 0x10000);  //   matrix.yx = - matrix.xy;
    matrix.yy = (FT_Fixed)(cos(angle) * 0x10000);  //   matrix.yy = matrix.xx;

  /* Place starting coordinates in adequate form. */
    pen.x = x_start * 64;
    pen.y = (int)(y_start / 64.0);

    int num_bytes = 0;
    while (text[num_bytes] != 0) {
       num_bytes++;
    }


    long *ucs4text;
    ucs4text = new long[num_bytes + 1];

    unsigned char u, v, w, x, y;

    int num_chars = 0;

    long iii = 0;

    while (iii < num_bytes) {
        unsigned char const z = text[iii];

        if (z <= 127) {
            ucs4text[num_chars] = z;
        }

        if ((192 <= z) && (z <= 223)) {
            iii++;
            y = text[iii];
            ucs4text[num_chars] = (z - 192) * 64 + (y - 128);
        }

        if ((224 <= z) && (z <= 239)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            ucs4text[num_chars] = (z - 224) * 4096 + (y - 128) * 64 + (x - 128);
        }

        if ((240 <= z) && (z <= 247)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            iii++;
            w = text[iii];
            ucs4text[num_chars] = (z - 240) * 262144 + (y - 128) * 4096 + (x - 128) * 64 + (w - 128);
        }

        if ((248 <= z) && (z <= 251)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            iii++;
            w = text[iii];
            iii++;
            v = text[iii];
            ucs4text[num_chars] = (z - 248) * 16777216 + (y - 128) * 262144 + (x - 128) * 4096 + (w - 128) * 64 + (v - 128);
        }

        if ((252 == z) || (z == 253)) {
            iii++;
            y = text[iii];
            iii++;
            x = text[iii];
            iii++;
            w = text[iii];
            iii++;
            v = text[iii];
            u = text[iii];
            ucs4text[num_chars] = (z - 252) * 1073741824 + (y - 128) * 16777216 + (x - 128) * 262144 + (w - 128) * 4096 + (v - 128) * 64 + (u - 128);
        }

        if ((z == 254) || (z == 255))
        {
            LOG_WARNING << "Problem with character: invalid UTF-8 data." << std::endl;
            return false;
        }
        iii++;
        num_chars++;
    }

    // num_chars now contains the number of characters in the string.
    /* Initialize FT Library object */
    error = FT_Init_FreeType(&library);
    if (error) {
        LOG_WARNING << "FreeType: Could not init Library." << std::endl;
        delete[] ucs4text;
        return false;
    }

    /* Initialize FT face object */
    error = FT_New_Face(library, font.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        LOG_WARNING << "FreeType: Font was opened, but type not supported." << std::endl;
        delete[] ucs4text;
        return false;
    }
    else if (error)
    {
        LOG_WARNING << "FreeType: Could not find or load font file." << std::endl;
        delete[] ucs4text;
        return false;
    }

    /* Set the Char size */
    error = FT_Set_Char_Size(face,          /* handle to face object           */
                             0,             /* char_width in 1/64th of points  */
                             fontsize * 64, /* char_height in 1/64th of points */
                             100,           /* horizontal device resolution    */
                             100);          /* vertical device resolution      */

    /* A way of accesing the glyph directly */
    FT_GlyphSlot slot = face->glyph; // a small shortcut

    /* Does the font file support kerning? */
    use_kerning = FT_HAS_KERNING(face);

    for (int n = 0; n < num_chars; n++) {
        /* Convert character code to glyph index */
        glyph_index = FT_Get_Char_Index(face, ucs4text[n]);

        /* Retrieve kerning distance and move pen position */
        if (use_kerning && previous && glyph_index) {
            FT_Vector delta;
            FT_Get_Kerning(face,
                           previous,
                           glyph_index,
                           ft_kerning_default, //FT_KERNING_DEFAULT,
                          &delta);

            /* Transform this kerning distance into rotated space */
            pen.x += (int)(((double)delta.x) * cos(angle));
            pen.y += (int)(((double)delta.x) * (sin(angle)));
        }

        /* Set transform */
        FT_Set_Transform(face, &matrix, &pen);

        if (error) {
            LOG_WARNING << "FreeType: Set char size error.";
            delete[] ucs4text;
            return false;
        }

        /* Retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index(face, ucs4text[n]);

        /* Load glyph image into the slot (erase previous one) */
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error) {
            LOG_WARNING << "FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error << ").";
            delete[] ucs4text;
            return false;
        }

        /* Convert to an anti-aliased bitmap */
        error = FT_Render_Glyph(face->glyph, ft_render_mode_normal);
        if (error) {
            LOG_WARNING << "FreeType: Render glyph error.";
            delete[] ucs4text;
            return false;
        }

        /* Now, draw to our target surface */
        drawBitmap(slot->bitmap_left, y_start + (fontsize - slot->bitmap_top),
                   slot->bitmap.buffer, slot->bitmap.width, slot->bitmap.rows,
                   color);

        /* Advance to the next position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;

        /* record current glyph index */
        previous = glyph_index;
    }

    /* Free the face and the library objects */
    FT_Done_Face(face);
    FT_Done_FreeType(library);

    delete[] ucs4text;
    return true;
}

void FrameBuffer::drawBitmap(uint32_t x, uint32_t y, uint8_t* bitmap, uint32_t width, uint32_t height, uint32_t color)
{
    uint32_t Bpp = bytesPerPixel();
    for (uint32_t i = 0; i < width; ++i) {
        for (uint32_t j = 0; j < height; ++j) {

            uint32_t pos = offset(x + i, y + j);
            if (bitmap[j * width + i] && pos != -1) {
                double alpha = (double)bitmap[j * width + i] / 255.;

                Color bg = convert(m_mem + pos);
                Color fg = convert(color);

                fg.r = alpha * (double) fg.r + (1. - alpha) * (double) bg.r;
                fg.g = alpha * (double) fg.g + (1. - alpha) * (double) bg.g;
                fg.b = alpha * (double) fg.b + (1. - alpha) * (double) bg.b;

                Color c = convert(fg.r << 16 | fg.g << 8 | fg.b);
                memcpy(m_mem + pos, c.data, Bpp);
            }
        }
    }
}

void FrameBuffer::drawPixmap(uint32_t x, uint32_t y, const Pixmap* pixmap)
{
    uint32_t Bpp = bytesPerPixel();
    for (uint32_t i = 0; i < pixmap->width(); ++i) {
        for (uint32_t j = 0; j < pixmap->height(); ++j) {

            uint32_t pos = offset(x + i, y + j);
            uint32_t px = pixmap->rgba(i, j);
            if (pos != -1) {
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

uint32_t FrameBuffer::drawQRCode(QRcode* code, uint32_t x, uint32_t y, uint32_t width, uint32_t color)
{
    Color c = convert(color);
    Color bg = convert(~color);
    uint32_t Bpp = bytesPerPixel();
    int len = floor((double)width / (double)code->width);
    for(uint32_t i = 0; i < code->width; ++i) {
        for(uint32_t j = 0; j < code->width; ++j) {
            if (code->data[i * code->width + j] & 1)
                fillRect(x + i * len, y + j * len, len, len, c);
        }
    }
    return len * code->width;
}

uint32_t FrameBuffer::qrCodeSize(QRcode* code, uint32_t width) const
{
    return floor((double)width / (double)code->width) * code->width;
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
