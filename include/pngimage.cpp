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

#include "pngimage.h"
#include <stdlib.h>
#include <stdio.h>

<<<<<<< HEAD
BEGIN_LIBFB_NS

PNGImage::PNGImage(std::string path)
    : m_width(0)
=======
using namespace IoT;

PNGImage::PNGImage(std::string path)
    : Pixmap()
    , m_width(0)
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
    , m_height(0)
    , m_row_pointers(NULL)
{
    if (!path.empty()) {
        open(path);
    }
}

PNGImage::~PNGImage()
{
    close();
}

bool PNGImage::open(std::string path)
{
    FILE *fp = fopen(path.c_str(), "rb");
    if (fp == NULL) {
        return false;
    }
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
        return false;
    };

    png_infop info = png_create_info_struct(png);
    if(!info) {
        return false;
    }

    if(setjmp(png_jmpbuf(png))){
        return false;
    }

    png_init_io(png, fp);

    png_read_info(png, info);

    m_width = png_get_image_width(png, info);
    m_height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if( color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    m_row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * m_height);
    for(int y = 0; y < m_height; y++) {
        m_row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, m_row_pointers);

    fclose(fp);
    return true;
}

<<<<<<< HEAD
color_t PNGImage::get(pos_t x, pos_t y) const
=======
uint32_t PNGImage::rgba(uint32_t x, uint32_t y) const
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
{
    if (m_height == 0 | m_width == 0) {
        return 0;
    }
    png_bytep row = m_row_pointers[y];
    png_bytep px = &(row[x * 4]);
    return (px[0] << 24) | (px[1] << 16) | (px[2] << 8) | px[3];
}

<<<<<<< HEAD
dimension_t PNGImage::width() const
=======
uint32_t PNGImage::width() const
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
{
    return m_width;
}

<<<<<<< HEAD
dimension_t PNGImage::height() const
=======
uint32_t PNGImage::height() const
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
{
    return m_height;
}

void PNGImage::close()
{
    for(int y = 0; y < m_height; y++) {
        if (m_row_pointers[y] != NULL) {
            free(m_row_pointers[y]);
            m_row_pointers[y] = NULL;
        }
    }
    if (m_row_pointers != NULL) {
        free(m_row_pointers);
        m_row_pointers = NULL;
    }
<<<<<<< HEAD
}

END_LIBFB_NS
=======
}
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
