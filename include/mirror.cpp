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

#include "mirror.h"

BEGIN_LIBFB_NS

Mirror::Mirror(const Drawable* source, bool horisontal, bool vertical)
    : DrawableProxy(source), m_h(horisontal), m_v(vertical)
{}

dimension_t Mirror::width() const
{
    return m_source->width();
}

dimension_t Mirror::height() const
{
    return m_source->height();
}

color_t Mirror::get(pos_t x, pos_t y) const
{
    if (x < width() && y < height())
    {
        pos_t mappedX = x;
        pos_t mappedY = y;
        if (m_h) {
            if (x > (width() / 2)) {
                mappedX = x - width();
            } else {
                mappedX = width() - x; 
            }
        }

        if (m_v) {
            if (y > (height() / 2)) {
                mappedX = x - height();
            } else {
                mappedX = height() - x; 
            }
        }

        return m_source->get(mappedX, mappedY);
    }

    return INVALID_COLOR;
}

END_LIBFB_NS