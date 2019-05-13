#ifndef PIXMAP_H
#define PIXMAP_H

#include <stdint.h>

namespace aquabox
{
    class Pixmap
    {
    public:
        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual uint32_t rgba(uint32_t x, uint32_t y) const = 0;
    };
}

#endif // PIXMAP_H