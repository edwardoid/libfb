#include "pngimage.h"
#include "pngimage.h"
#include "pngimage.h"
#include <stdlib.h>
#include <stdio.h>

using namespace fb;

PNGImage::PNGImage(std::string path)
    : Pixmap()
    , m_width(0)
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

uint32_t PNGImage::rgba(uint32_t x, uint32_t y) const
{
    if (m_height == 0 | m_width == 0) {
        return 0;
    }
    png_bytep row = m_row_pointers[y];
    png_bytep px = &(row[x * 4]);
    return (px[0] << 24) | (px[1] << 16) | (px[2] << 8) | px[3];
}

uint32_t PNGImage::width() const
{
    return m_width;
}

uint32_t PNGImage::height() const
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
}