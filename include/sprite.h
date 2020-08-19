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
#include <memory>

BEGIN_LIBFB_NS
class Sprite: public Drawable
{
public:
    Sprite(std::shared_ptr<Drawable> source, dimension_t frameWidth, dimension_t frameHeight, uint32_t framesCount);
    uint32_t frame() const;
    void jump(uint32_t idx);
    uint32_t framesCount() const;
    virtual color_t get(pos_t x, pos_t y) const override;
    dimension_t width() const override;
    dimension_t height() const override;
private:
    std::shared_ptr<Drawable> m_source;
    uint32_t m_pos;
    dimension_t m_frameWidth;
    dimension_t m_frameHeight;
    uint32_t m_framesCount;
};

END_LIBFB_NS
