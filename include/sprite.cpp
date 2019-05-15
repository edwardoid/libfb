#include "sprite.h"
#include "log.h"

using namespace fb;

Sprite::Sprite(std::shared_ptr<Pixmap> source, uint32_t frameWidth, uint32_t frameHeight, uint32_t framesCount)
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

uint32_t Sprite::rgba(uint32_t x, uint32_t y) const
{
    if (m_framesCount == 0) {
        return 0;
    }

    return m_source->rgba(m_pos * m_frameWidth + x, y);
}

uint32_t Sprite::width() const
{
    return m_frameWidth;
}

uint32_t Sprite::height() const
{
    return m_frameHeight;
}