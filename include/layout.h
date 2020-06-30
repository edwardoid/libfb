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

#include "drawable.h"


BEGIN_LIBFB_NS

class Layout: public Drawable
{
public:
	Layout(Drawable& m_root);
	int id(const Drawable& drawable);
	void add(Drawable& d);
	void anchorLeft(int left, int right);
	void anchorRight(int right, int left);
	void anchorTop(int top, int bottom);
	void anchorBottom(int bottom, int top);
};

END_LIBFB_NS