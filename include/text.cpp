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

#include "text.h"
#include "helpers.h"
#include <cmath>
#include <limits>
#include "log.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#define DEBUG std::cout << "\n" << __FILE__ << ":" << __LINE__ << " "

BEGIN_LIBFB_NS

struct Rect
{
    int32_t left = std::numeric_limits<int32_t>::max();
    int32_t top = std::numeric_limits<int32_t>::max();
    int32_t right = std::numeric_limits<int32_t>::min();
    int32_t bottom = std::numeric_limits<int32_t>::min();
    inline void includePoint(int32_t x, int32_t y)
    {
        left = std::min(x, left);
        right = std::max(right, x);
        top = std::min(top, y);
        bottom = std::max(bottom, y);
    }

    inline void includeRect(int32_t x, int32_t y, int32_t w, int32_t h)
    {
        includePoint(x, y);
        // includePoint(x + w, y);
        // includePoint(x, y + h);
        includePoint(x + w, y + h);
    }

    inline int32_t w() const { return right - left; }
    inline int32_t h() const { return bottom - top; }

    inline void moveTo(int32_t x, int32_t y)
    {
        int32_t w = Rect::w();
        int32_t h = Rect::h();
        left = x;
        top = y;
        right = x + w;
        bottom = y + h;
    }

    inline bool isValid() const
    {
        return left < right &&
               bottom < top;
    }

    inline std::string toString()
    {
        std::stringstream ss;
        ss << "[(" << left << ", " << top << ") (" << right << ", " << bottom << ")] w = " << w() << " h = " << h();
        return ss.str();
    }
};

Text::Text()
    : m_buffer(nullptr)
{

}

Text::~Text()
{
    
}

void Text::destroyBuffer()
{
    if (m_buffer != nullptr)
    {
        for(size_t i = 0; i < m_bufferRows; ++i)
        {
            delete [] m_buffer[i];
        }

        delete []m_buffer;
    }

    m_buffer = nullptr;
    m_bufferRows = 0;
    m_bufferColumns = 0;
}

void Text::resizeBuffer(size_t width, size_t height)
{
    destroyBuffer();
    if (width == 0 || height == 0)
    {
        return;
    }
    m_buffer = new uint8_t* [height];
    m_bufferRows = height;
    m_bufferColumns = width;
    
    for (int i = 0; i < height; ++i)
    {
        m_buffer[i] = new uint8_t[width];
        memset(m_buffer[i], 0, width);
    }
}

void Text::setText(const std::string& text)
{
    if (m_text != text)
    {
        m_text = text;
        update();
    }
}

void Text::setFont(const std::string& font)
{
    if (m_font != font)
    {
        m_font = font;
        update();
    }
}

void Text::setSize(dimension_t size)
{
    if (m_fontSize != size)
    {
        m_fontSize = size;
        update();
    }
}

void Text::setColor(color_t color)
{
    if (m_color != color)
    {
        m_color = color;
        update();
    }
}

void Text::setAngle(double angle)
{
    if (m_angle != angle)
    {
        m_angle = angle;
        update();
    }
}

void Text::mapTo(pos_t x, pos_t y)
{
    Drawable::mapTo(x, y);
    if (m_x != x || m_y != y)
    {
        update();
    }
}

void Text::setDPI(dimension_t dpi)
{
    if (m_dpi != dpi) {
        m_dpi = dpi;
        update();
    }
}

void putBitmap(uint8_t** buffer, FT_Bitmap * bitmap, pos_t x, pos_t y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;


    /* for simplicity, we assume that `bitmap->pixel_mode' */
    /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

    for ( i = x, p = 0; i < x_max; i++, p++ )
    {
        for ( j = y, q = 0; j < y_max; j++, q++ )
        {
            if (bitmap->buffer[q * bitmap->width + p])
                buffer[j][i] = bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void Text::update()
{
    if (m_text.empty() ||
        m_font.empty() ||
        m_fontSize == 0 ||
        m_color == INVALID_COLOR ||
        m_dpi < 1)
    {
        destroyBuffer();
        return;
    }

    FT_Library library;
    if(FT_Init_FreeType(&library))              /* initialize library */
    {
        return;
    }

    FT_Face       face;
    if(FT_New_Face(library, m_font.c_str(), 0, &face)) /* create face object */
    {
        FT_Done_FreeType(library);
        return;
    }
    /* error handling omitted */

    if (FT_Set_Char_Size( face, m_fontSize * 64, 0, m_dpi, m_dpi ))
    {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return;
    }
    /* error handling omitted */

    /* cmap selection omitted;                                        */
    /* for simplicity we assume that the font contains a Unicode cmap */

    FT_GlyphSlot slot = face->glyph;

    /* set up matrix */
    FT_Matrix matrix;
    matrix.xx = (FT_Fixed)( cos( m_angle ) * 0x10000L );
    matrix.xy = (FT_Fixed)(-sin( m_angle) * 0x10000L );
    matrix.yx = (FT_Fixed)( sin( m_angle ) * 0x10000L );
    matrix.yy = (FT_Fixed)( cos( m_angle ) * 0x10000L );

    FT_Vector pen;
    pen.x = 0;
    pen.y = 0;

    FT_UInt glyph_index;

    FT_Bool use_kerning = FT_HAS_KERNING(face);

    FT_UInt previous = 0;

    Rect bound;

    for (size_t n = 0; n < m_text.size(); n++)
    {
        glyph_index = FT_Get_Char_Index(face, m_text[n]);

        /* Retrieve kerning distance and move pen position */
        if (use_kerning && previous && glyph_index) {
            FT_Vector delta;
            FT_Get_Kerning(face,
                           previous,
                           glyph_index,
                           ft_kerning_default, //FT_KERNING_DEFAULT,
                          &delta);

            /* Transform this kerning distance into rotated space */
            pen.x += (int)(((double)delta.x) * cos(m_angle));
            pen.y += (int)(((double)delta.x) * (sin(m_angle)));
        }

        /* set transformation */
        FT_Set_Transform( face, &matrix, &pen );

        if (FT_Load_Char( face, m_text[n], FT_LOAD_RENDER )) {
            continue;                /* ignore errors */
        }

        /* Convert to an anti-aliased bitmap */
        FT_Render_Glyph(face->glyph, ft_render_mode_mono);

        int32_t overOrigin = pen.y + face->glyph->metrics.horiBearingY;
        int32_t underOrigin = pen.y - (face->glyph->metrics.height - face->glyph->metrics.horiBearingY);

        if (slot->bitmap.width != 0 && slot->bitmap.rows != 0) {
            bound.includeRect(pen.x + face->glyph->metrics.horiBearingX , -overOrigin,
                              face->glyph->metrics.horiAdvance, face->glyph->metrics.height);
        }

        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }


    int32_t shiftX = bound.left;
    int32_t underOrigin = bound.bottom;
    int32_t overOrigin = bound.h() - underOrigin;

    int16_t w = bound.w() >> 6;
    int16_t h = bound.h() >> 6;

    resizeBuffer(w, h);

    previous = 0;

    FT_Vector drawPen;
    drawPen.x = 0;
    drawPen.y = 0;

    for (int n = 0; n < m_text.size();n++ )
    {
        glyph_index = FT_Get_Char_Index(face, m_text[n]);

        /* Retrieve kerning distance and move pen position */
        if (use_kerning && previous && glyph_index) {
            FT_Vector delta;
            FT_Get_Kerning(face,
                           previous,
                           glyph_index,
                           ft_kerning_default, //FT_KERNING_DEFAULT,
                          &delta);

            /* Transform this kerning distance into rotated space */
            drawPen.x += (int)(((double)delta.x) * cos(m_angle));
            drawPen.y += (int)(((double)delta.x) * (sin(m_angle)));
        }

        /* set transformation */
        FT_Set_Transform( face, &matrix, &drawPen );

        /* Retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index(face, m_text[n]);

        /* Load glyph image into the slot (erase previous one) */
        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER))
        {
            continue;
        }

        /* Convert to an anti-aliased bitmap */
        if (FT_Render_Glyph(face->glyph, ft_render_mode_mono)) {
            continue;
        }

        /* now, draw to our target surface (convert position) */
        Rect glyphBound;
        glyphBound.includeRect(drawPen.x + (face->glyph->metrics.horiBearingX >> 6),
                               (overOrigin - face->glyph->metrics.horiBearingY) >> 6,
                               slot->bitmap.width, slot->bitmap.rows);

        //glyphBound.moveTo(drawPen.x >> 6, drawPen.y >> 6);

        putBitmap(  m_buffer, &slot->bitmap,
                    glyphBound.left, glyphBound.top);

        /* increment pen position */
        drawPen.x += slot->advance.x >> 6;
        drawPen.y += slot->advance.y >> 6;

        /* record current glyph index */
        previous = glyph_index;
    }

    FT_Done_Face( face );
    FT_Done_FreeType( library );
}

dimension_t Text::width() const
{
    return m_bufferColumns;
}

dimension_t Text::height() const
{
    return m_bufferRows;
}

color_t Text::get(pos_t x, pos_t y) const
{
    if (x < width() && y < height())
    {
        uint8_t op = std::round(double(helpers::a(m_color) * m_buffer[y][x]) / 255.);
        color_t c = m_color;
        c = (c >> 8) << 8;
        return c + op;
    }
    return INVALID_COLOR;
}

END_LIBFB_NS