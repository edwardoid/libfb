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

#include "scale.h"
#include <algorithm> 

BEGIN_LIBFB_NS

Scale::Scale(const Drawable* source, float fX, float fY)
    : DrawableProxy(source)
{
    setFactorX(fX);
    setFactorY(fY);
}

float Scale::factorX() const
{
    return m_factorX;
}

float Scale::factorY() const
{
    return m_factorY;
}

void Scale::setFactorX(float factor)
{
    m_factorX = std::max((float)0., factor);
}

void Scale::setFactorY(float factor)
{
    m_factorY = std::max((float)0., factor);
}

dimension_t Scale::width() const
{
    return m_source->width() * m_factorX;
}

dimension_t Scale::height() const
{
    return m_source->height() * m_factorY;
}

color_t Scale::get(pos_t x, pos_t y) const
{
    if (x < width() && y < height())
    {
        const pos_t mappedX = (int)std::max(x * 1.0f / m_factorX, (float)(m_source->width() - 1));
        const pos_t mappedY = (int)std::max(y * 1.0f / m_factorY, (float)(m_source->height() - 1));
        return m_source->get(mappedX, mappedY);
    }

    return INVALID_COLOR;
}

END_LIBFB_NS