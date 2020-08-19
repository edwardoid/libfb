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

<<<<<<< HEAD
#pragma once
=======
#ifndef PNG_PROVIDER_H
#define PNG_PROVIDER_H
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd

#include <string>
#include <unordered_map>
#include <memory>
#include "pngimage.h"

<<<<<<< HEAD
BEGIN_LIBFB_NS
class PNGProvider
{
public:
    std::shared_ptr<PNGImage> get(std::string path);
private:
    std::unordered_map<std::string, std::shared_ptr<PNGImage>> m_cache;
};

END_LIBFB_NS
=======
namespace IoT
{
    class PNGProvider
    {
    public:
        std::shared_ptr<PNGImage> get(std::string path);
    private:
        std::unordered_map<std::string, std::shared_ptr<PNGImage>> m_cache;
    };
}

#endif // PNG_PROVIDER_H
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
