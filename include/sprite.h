#ifndef SPRITE_H
#define SPRITE_H

#include "pixmap.h"
#include <memory>

namespace fb
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