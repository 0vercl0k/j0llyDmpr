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
#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <windows.h>

#ifdef DEBUG
        #define FILE_LOG "j0llyDmp3r.log"
        extern FILE *fpDbgManager;

        #define DEBUGMSG(...)                    \
        {                                        \
            fprintf(fpDbgManager, __VA_ARGS__);  \
            fflush(fpDbgManager);                \
        }

        #define InitDbgManager()                 \
            initDbgManager()

        #define CloseDbgManager()                \
            fclose(fpDbgManager)
#else
    #define DEBUGMSG(...) /* void */
    #define InitDbgManager() /* void */
    #define CloseDbgManager() /* void */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* # Initialization of debug-manager */
UCHAR initDbgManager();

#ifdef __cplusplus
}
#endif

#endif
