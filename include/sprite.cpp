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

#include "sprite.h"
#include "log.h"

<<<<<<< HEAD
BEGIN_LIBFB_NS

Sprite::Sprite(std::shared_ptr<Drawable> source, dimension_t frameWidth, dimension_t frameHeight, uint32_t framesCount)
=======
using namespace IoT;

Sprite::Sprite(std::shared_ptr<Pixmap> source, uint32_t frameWidth, uint32_t frameHeight, uint32_t framesCount)
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
    : m_source(source)
    , m_framesCount(framesCount)
    , m_frameWidth(0)
    , m_frameHeight(0)
    , m_pos(0)
{
    if (m_source == nullptr) {
        LOG_ERROR << "No Source image provided!";
        return;
    }
    m_frameWidth = source->width() < frameWidth ? source->width() : frameWidth;
    m_frameHeight = source->height() < frameHeight ? source->height() : frameHeight;
}

uint32_t Sprite::frame() const
{
    return m_pos;
}

void Sprite::jump(uint32_t idx)
{
    if (m_framesCount == 0) {
        return;
    }
    m_pos = idx % m_framesCount;
}

uint32_t Sprite::framesCount() const
{
    return m_framesCount;
}

<<<<<<< HEAD
color_t Sprite::get(pos_t x, pos_t y) const
=======
uint32_t Sprite::rgba(uint32_t x, uint32_t y) const
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
{
    if (m_framesCount == 0) {
        return 0;
    }

<<<<<<< HEAD
    return m_source->get(m_pos * m_frameWidth + x, y);
}

dimension_t Sprite::width() const
=======
    return m_source->rgba(m_pos * m_frameWidth + x, y);
}

uint32_t Sprite::width() const
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
{
    return m_frameWidth;
}

<<<<<<< HEAD
dimension_t Sprite::height() const
{
    return m_frameHeight;
}

END_LIBFB_NS
=======
uint32_t Sprite::height() const
{
    return m_frameHeight;
}
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
