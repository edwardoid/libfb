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

#include "pngprovider.h"

<<<<<<< HEAD
BEGIN_LIBFB_NS
=======
using namespace IoT;
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd

std::shared_ptr<PNGImage> PNGProvider::get(std::string path)
{
    auto pos = m_cache.find(path);
    if (pos != m_cache.end()) {
        return pos->second;
    }

    std::shared_ptr<PNGImage> p = std::make_shared<PNGImage>();
    if (!p->open(path)) {
        return nullptr;
    }
    m_cache.emplace(path, p);

    return m_cache[path];
}
<<<<<<< HEAD

END_LIBFB_NS
=======
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
