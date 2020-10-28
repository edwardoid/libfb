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

BEGIN_LIBFB_NS

class Text: public Drawable
{
public:
    Text();
    ~Text();
    virtual dimension_t width() const override;
    virtual dimension_t height() const override;
    virtual color_t get(pos_t x, pos_t y) const override;
    virtual void mapTo(pos_t x, pos_t y) override;
    void setText(const std::string& text);
    void setFont(const std::string& font);
    void setSize(dimension_t size);
    void setColor(color_t color);
    void setAngle(double angle);
    void setDPI(dimension_t dpi);
private:
    void update();
    void destroyBuffer();
    void resizeBuffer(size_t width, size_t height);
private:
    delta_t m_fontSize = 0;
    dimension_t m_dpi = 100;
    std::string m_font;
    std::string m_text;
    double m_angle;
    color_t m_color = INVALID_COLOR;
    size_t m_bufferRows = 0;
    size_t m_bufferColumns = 0;
    uint8_t** m_buffer;
};

END_LIBFB_NS