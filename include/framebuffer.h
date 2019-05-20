#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <string>
#include <stdint.h>
#include <qrencode.h>
#include <linux/fb.h>
#include "pixmap.h"

namespace fb
{

    class FrameBuffer
    {
    public:
        FrameBuffer(std::string device);
        ~FrameBuffer();
        uint32_t bitsPerPixel() const;
        uint32_t bytesPerPixel() const;
        bool open();
        bool close();
        void clear(uint32_t color = 0x000000);
        void flush();

        void drawPoint(uint32_t x, uint32_t y, uint32_t rgb);
        void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t rgb);
        void drawBitmap(uint32_t x, uint32_t y, uint8_t* bitmap, uint32_t width, uint32_t height, uint32_t color);
        void drawPixmap(uint32_t x, uint32_t y, const Pixmap* pixmap);
        uint32_t drawQRCode(QRcode* code, uint32_t x, uint32_t y, uint32_t width, uint32_t color);
        uint32_t qrCodeSize(QRcode* code, uint32_t width) const;
        void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
        void drawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uin;t32_t color);
        bool drawText(std::string fontPath, uint32_t fontsize, uint32_t x, uint32_t y, double angle, std::string text, uint32_t color);

        uint32_t width() const;
        uint32_t height() const;
        void refresh();

    private:
        struct Color
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        protected:
            friend class FrameBuffer;
            uint8_t data[3];
        };

        Color convert(uint32_t rgb);
        Color convert(uint8_t* pos);

        uint32_t offset(uint32_t x, uint32_t y);
        void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Color& c);
        void drawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const Color& c);
    private:
        std::string m_device;
        int m_fbd;
        struct fb_var_screeninfo m_varInfo;
        struct fb_fix_screeninfo m_fixInfo;
        uint8_t* m_mem;
        uint8_t* m_hw;
        uint32_t m_len;
    };
}

#endif // FRAMEBUFFER_H
