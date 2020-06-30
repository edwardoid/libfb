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

#include <drawable.h>
#include <helpers.h>
#include <qrencode.h>
#include <string>

BEGIN_LIBFB_NS

class QRCode: public Drawable
{
public:
    QRCode() = default;
    virtual ~QRCode();
    void resizeTo(dimension_t desiredSize);
    void setText(std::string data);
    void setCorrectionLevel(QRecLevel level);
    void setMode(QRencodeMode mode);
    dimension_t size() const;
    virtual dimension_t width() const;
    virtual dimension_t height() const;
    virtual color_t get(pos_t x, pos_t y) const;
    void setColors(color_t foreground, color_t background = helpers::c(0xFF, 0xFF, 0xFF, 0));
private:
    void update();
private:
    QRecLevel m_level = QRecLevel::QR_ECLEVEL_M;
    QRencodeMode m_mode = QRencodeMode::QR_MODE_8;
    std::string m_text;
    dimension_t m_desiredSize = 1;
    QRcode* m_data = nullptr;
    color_t m_fg = 0x000000FF;
    color_t m_bg = 0xFFFFFFFF;
};

END_LIBFB_NS