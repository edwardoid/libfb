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

#include "drawable.h"
#include "log.h"
#include <stdio.h>
#include <malloc.h>
#include <png.h>

BEGIN_LIBFB_NS

pos_t Drawable::x() const
{
    return m_x;
}

pos_t Drawable::y() const
{
    return m_y;
}

void Drawable::mapTo(pos_t x, pos_t y)
{
    m_x = x;
    m_y = y;
}

DrawableProxy::DrawableProxy(const Drawable* source)
    : m_source(source)
{
    m_x = source->x();
    m_y = source->y();
}

void Drawable::dumpToImage(std::string filename) const
{
    int code = 0;

    FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	color_t* row = NULL;

    try {
        fp = fopen(filename.c_str(), "wb");
        if (fp == NULL) {
            LOG_ERROR <<  "Could not open file " << filename << " for writing\n";
            throw 1;
        };
        
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == NULL) {
            LOG_ERROR << "Could not allocate write struct";
            throw 1;
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            LOG_ERROR << "Could not allocate info struct";
            throw 1;
        }
        color_t* row = NULL;

        if (setjmp(png_jmpbuf(png_ptr))) {
            LOG_ERROR << "Error during png creation";
            throw 1;
        }
        png_init_io(png_ptr, fp);

        // Write header (8 bit colour depth)
        png_set_IHDR(png_ptr, info_ptr, width(), height(),
                8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        // // Set title
        // if (title != NULL) {
        //     png_text title_text;
        //     title_text.compression = PNG_TEXT_COMPRESSION_NONE;
        //     title_text.key = "Title";
        //     title_text.text = title;
        //     png_set_text(png_ptr, info_ptr, &title_text, 1);
        // }

        png_write_info(png_ptr, info_ptr);

        // Allocate memory for one row (3 bytes per pixel - RGB)
        row = new color_t[width()];

        // Write image data
        int x, y;
        for (y=0 ; y< height() ; y++) {
            for (x=0 ; x<width() ; x++) {
                row[x] = get(x, y);
            }
            png_byte* p = reinterpret_cast<png_byte*>(row);
            png_write_row(png_ptr, p);
        }

        // End write
        png_write_end(png_ptr, NULL);

        if (fp != NULL) fclose(fp);
        if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        if (row != NULL) delete []row;
    }
    catch(int code) {
        if (fp != NULL) fclose(fp);
        if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        if (row != NULL) delete []row;
    }
}

END_LIBFB_NS