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
#include <vector>

BEGIN_LIBFB_NS

class Scale: public DrawableProxy
{
public:
	Scale(const Drawable* d, float factorX = 1., float factorY = 1.);
    float factorX() const;
    float factorY() const;
    void setFactorX(float factor = 1.);
    void setFactorY(float factor = 1.);
    virtual dimension_t width() const override;
    virtual dimension_t height() const override;
    virtual color_t get(pos_t x, pos_t y) const override;
private:
    color_t average(std::vector<color_t> colors);
private:
    float m_factorX = 1.;
    float m_factorY = 1.;
};

END_LIBFB_NS