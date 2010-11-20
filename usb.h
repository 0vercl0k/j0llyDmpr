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
#ifndef _USB_H
#define _USB_H

#include <windows.h>
#include "global.h"

/** Prototypes **/

#ifdef __cplusplus
extern "C" {
#endif

/* # Get the volume letter of the USB key just connected */
UCHAR GetLetterOfNewVolume(const DWORD precVal, const DWORD newVal);

/* # Search and copy interessting files */
BOOL DumpAndSearchInteresstingFiles(const PUCHAR pVol, const DWORD lvl, PCONFIG pConf);

/* # Filename match with patterns */
BOOL isAnInteresstingFile(const PUCHAR file, const DWORD64 fileSize, PCONFIG pConf);

/* # Initialization stuff */
VOID initUsbStuff(const char* outpath);

#ifdef __cplusplus
}
#endif

#endif
