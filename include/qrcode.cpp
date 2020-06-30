#include "qrcode.h"
#include <math.h>

BEGIN_LIBFB_NS

void QRCode::update()
{
    QRcode *qrcode = QRcode_encodeString(m_text.c_str(), 0, m_level, m_mode, 1);
	if (qrcode == nullptr) {
        return;
    }

    if (m_data != nullptr) {
        delete m_data;
    }

    m_data = qrcode;
}

void QRCode::setText(std::string text)
{
    m_text = text;
    update();
}

dimension_t QRCode::width() const
{
    return QRCode::height();
}

dimension_t QRCode::height() const
{
    return size();
}

dimension_t QRCode::size() const
{
    return m_data == nullptr ? 0 : (floor((double)m_desiredSize / (double)m_data->width) * m_data->width);
}

void QRCode::setColors(color_t foreground, color_t background)
{
    m_fg = foreground;
    m_bg = background;
}

color_t QRCode::get(pos_t x, pos_t y) const
{
    auto sz = size();
    if (x < sz && y < sz)
    {
        int len = floor((double)sz / (double)m_data->width);
        x /= len;
        y /= len;
        if (m_data->data[y * m_data->width + x] & 1) {
            return m_fg;
        }
        return m_bg;
    }

    return INVALID_COLOR;
}

void QRCode::resizeTo(dimension_t desiredSize)
{
    m_desiredSize = desiredSize;
}

QRCode::~QRCode()
{
    if (m_data != nullptr) {
        delete m_data;
    }
}

END_LIBFB_NS