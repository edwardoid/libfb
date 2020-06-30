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

END_LIBFB_NS