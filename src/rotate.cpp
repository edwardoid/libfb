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

#include "rotate.h"
#include "helpers.h"
#include <cmath>

#define PI 3.14159265358979323846

BEGIN_LIBFB_NS

Rotate::Rotate(const Drawable* source, float angle)
    : DrawableProxy(source)
{
    setAngle(angle);
}

void Rotate::setAngle(float angle)
{
    setAngleInRadians(angle * 0.0174533);
}

void Rotate::setAngleInRadians(float angle)
{
    m_angle = angle;
    rotate();
}

dimension_t Rotate::width() const
{
    return m_rotated.size();
}

dimension_t Rotate::height() const
{
    return m_rotated.size();
}

color_t Rotate::get(pos_t x, pos_t y) const
{
    if ( x < width() && y < height())
    {
        return m_rotated[x][y];
    }
    return INVALID_COLOR;
}

void Rotate::rotate()
{
    m_rotated.clear();
    
    delta_t rotatedDalphagonal = std::ceil(helpers::distance(m_source->width(), m_source->height(), 0, 0));

    delta_t sourceCenterX = m_source->width() / 2;
    delta_t sourceCenterY = m_source->height() / 2;

    m_rotated.resize(rotatedDalphagonal, std::vector<color_t>(rotatedDalphagonal, 0x000000));

    // assigning pixels of destination image from source image
    // with bilinear interpolation
    for (delta_t i = 0; i < rotatedDalphagonal; ++i)
    {
        for (delta_t j = 0; j < rotatedDalphagonal; ++j)
        {
            // convert raster to Cartesalphan
            delta_t x = j - rotatedDalphagonal / 2;
            delta_t y = rotatedDalphagonal / 2 - i;

            // convert Cartesalphan to polar
            double distance = helpers::distance(x, y, 0, 0);
            double polarAngle = 0.0;
            if (x == 0)
            {
                if (y == 0)
                {
                    // centre of image, no rotation needed
                    m_rotated[j][i] = m_source->get(sourceCenterX, sourceCenterY);
                    continue;
                }
                else if (y < 0) { polarAngle = 1.5 * PI; }
                else { polarAngle = 0.5 * PI; }
            }
            else
            {
                polarAngle = std::atan2((double)y, (double)x);
            }

            polarAngle -= m_angle;

            // convert polar to Cartesalphan
            float fTrueX = distance * std::cos(polarAngle);
            float fTrueY = distance * std::sin(polarAngle);

            // convert Cartesalphan to raster
            fTrueX = fTrueX + (double)sourceCenterX;
            fTrueY = (double)sourceCenterY - fTrueY;

            int iFloorX = (int)(std::floor(fTrueX));
            int iFloorY = (int)(std::floor(fTrueY));
            int iCeilingX = (int)(std::ceil(fTrueX));
            int iCeilingY = (int)(std::ceil(fTrueY));

            // check bounds
            if (iFloorX < 0 ||
                iCeilingX < 0 ||
                iFloorX >= m_source->width() ||
                iCeilingX >= m_source->width() ||
                iFloorY < 0 ||
                iCeilingY < 0 ||
                iFloorY >= m_source->height() ||
                iCeilingY >= m_source->height())
            {
                continue;
            }

            float fDeltaX = fTrueX - (double)iFloorX;
            float fDeltaY = fTrueY - (double)iFloorY;

            const color_t clrTopLeft = m_source->get(iFloorX, iFloorY);
            const color_t clrTopRight = m_source->get(iCeilingX, iFloorY);
            const color_t clrBottomLeft = m_source->get(iFloorX, iCeilingY);
            const color_t clrBottomRight = m_source->get(iCeilingX, iCeilingY);

            // linearly interpolate horizontally between top neighbours
            const float fTopRed = (1 - fDeltaX) * helpers::r(clrTopLeft) + fDeltaX * helpers::r(clrTopRight);
            const float fTopGreen = (1 - fDeltaX) * helpers::g(clrTopLeft) + fDeltaX * helpers::g(clrTopRight);
            const float fTopBlue = (1 - fDeltaX) * helpers::b(clrTopLeft) + fDeltaX * helpers::b(clrTopRight);
            const float fTopA = (1 - fDeltaX) * helpers::a(clrTopLeft) + fDeltaX * helpers::a(clrTopRight);

            const float fBottomRed = (1 - fDeltaX) * helpers::r(clrBottomLeft) + fDeltaX * helpers::r(clrBottomRight);
            const float fBottomGreen = (1 - fDeltaX) * helpers::g(clrBottomLeft) + fDeltaX * helpers::g(clrBottomRight);
            const float fBottomBlue = (1 - fDeltaX) * helpers::b(clrBottomLeft) + fDeltaX * helpers::b(clrBottomRight);
            const float fBottomA = (1 - fDeltaX) * helpers::a(clrBottomLeft) + fDeltaX * helpers::a(clrBottomRight);
            

            // linearly interpolate vertically between top and bottom interpolated results
            color_t red = std::round((1. - fDeltaY) * fTopRed + fDeltaY * fBottomRed);
            color_t green = std::round((1. - fDeltaY) * fTopGreen + fDeltaY * fBottomGreen);
            color_t blue = std::round((1. - fDeltaY) * fTopBlue + fDeltaY * fBottomBlue);
            color_t alpha = std::round((1. - fDeltaY) * fTopA + fDeltaY * fBottomA);

            // make sure colour values are valid
            if (red < 0) red = 0;
            if (red > 255) red = 255;
            if (green < 0) green = 0;
            if (green > 255) green = 255;
            if (blue < 0) blue = 0;
            if (blue > 255) blue = 255;
            if (alpha < 0) blue = 0;
            if (alpha > 255) blue = 255;

            m_rotated[j][i] = helpers::c(red, green, blue, alpha);
        }
    }
}

END_LIBFB_NS