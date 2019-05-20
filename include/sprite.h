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

#ifndef SPRITE_H
#define SPRITE_H

#include "pixmap.h"
#include <memory>

namespace IoT
{
    class Sprite: public Pixmap
    {
    public:
        Sprite(std::shared_ptr<Pixmap> source, uint32_t frameWidth, uint32_t frameHeight, uint32_t framesCount);
        uint32_t frame() const;
        void jump(uint32_t idx);
        uint32_t framesCount() const;
        uint32_t rgba(uint32_t x, uint32_t y) const override;
        uint32_t width() const override;
        uint32_t height() const override;
    private:
        std::shared_ptr<Pixmap> m_source;
        uint32_t m_pos;
        uint32_t m_frameWidth;
        uint32_t m_frameHeight;
        uint32_t m_framesCount;
    };
}

#endif // SPRITE_H