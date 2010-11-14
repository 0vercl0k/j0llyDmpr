/**
 *   j0llyDmpr
 *   Copyright (C) 2010 Souchet Axel <0vercl0k@tuxfamily.org>
 *
 *   This file is part of j0llyDmpr.
 *
 *   J0llyDmpr is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   J0llyDmpr is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with J0llyDmpr. If not, see <http://www.gnu.org/licenses/>.
**/
#ifndef _COLOR_SHELL_H
#define _COLOR_SHELL_H

#include <windows.h>

typedef enum Color
{
    White = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
    Blue = FOREGROUND_BLUE,
    Red = FOREGROUND_RED,
    Green = FOREGROUND_GREEN
} COLOR;


#ifdef __cplusplus
extern "C" {
#endif

#define InitColor(){ \
    initColor();     \
    SetColorWhite(); \
}

#define SetColorGreen() setColor(Green)
#define SetColorWhite() setColor(White)
#define SetColorBlue()  setColor(Blue)
#define SetColorRed()   setColor(Red)


/* # Init color stuff */
VOID initColor();

/* # Set shell color at 'fg' color */
VOID setColor(const COLOR fg);


#ifdef __cplusplus
}
#endif

#endif
