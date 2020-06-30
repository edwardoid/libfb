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

#ifndef LOG_H
#define LOG_H

#include <iostream>
#define F_NAME __PRETTY_FUNCTION__
//#define LOG_FMT " In:\n" << __FILE__ << ":" << __LINE__ << " " << F_NAME << ":\n\t"
#define LOG_FMT " " << F_NAME << ":"

#define LOG_DEBUG std::cout << "\nDBG" << LOG_FMT
#define LOG_INFO  std::cout << "\nINF" << LOG_FMT
#define LOG_ERROR std::cerr << "\nERR" << LOG_FMT
#define LOG_WARN  std::cout << "\nWRN" << LOG_FMT
#define LOG_WARNING LOG_WARN
#endif // LOG_H
