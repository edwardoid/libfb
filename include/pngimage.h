#ifndef PNG_IMAGE_H
#define PNG_IMAGE_H

#include "pixmap.h"
#include <string>
#include <stdint.h>
#include <png.h>

namespace fb
{
    class PNGImage: public Pixmap
    {
    public:
        PNGImage(std::string path = "");
        ~PNGImage();
        bool open(std::string path);
        void close();
        uint32_t rgba(uint32_t x, uint32_t y) const override;
        uint32_t width() const override;
        uint32_t height() const override;
    private:
        uint32_t m_width;
        uint32_t m_height;
        png_bytep *m_row_pointers;
    };
}


#endif // PNG_IMAGE_H