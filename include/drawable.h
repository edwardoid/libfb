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

#include <libfb_globals.h>
#include <string>

BEGIN_LIBFB_NS

class Drawable
{
public:
    virtual pos_t x() const;
    virtual pos_t y() const;
    virtual void mapTo(pos_t x, pos_t y);
    virtual dimension_t width() const = 0;
    virtual dimension_t height() const = 0;
    virtual color_t get(pos_t x, pos_t y) const = 0;
    void dumpToImage(std::string filename) const;
protected:
    pos_t m_x = 0;
    pos_t m_y = 0;
};

class DrawableProxy: public Drawable
{
public:
    DrawableProxy(const Drawable* source);

protected:
    const Drawable* m_source;
};

END_LIBFB_NS
