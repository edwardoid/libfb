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

#include <libfb_globals.h>
#include <map>
#include <vector>

BEGIN_LIBFB_NS

class Drawable;

typedef std::vector<Drawable*> Layer;

class Canvas
{
public:
    using zorder_t = int8_t;
    Canvas(dimension_t width, dimension_t height);
    dimension_t width() const;
    dimension_t height() const;

    Layer& addLayer(zorder_t z, Drawable* item = nullptr);
    void dropLayer(zorder_t);
	bool hasLayer(zorder_t z) const;
	Layer& layer(zorder_t z);
private:
    std::map<zorder_t, Layer> m_layers;
};

END_LIBFB_NS
