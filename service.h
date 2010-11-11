/**
 *   j0llyDmpr
 *   Copyright (C) 2010 0vercl0k <0vercl0k@tuxfamily.org>
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
#ifndef _SERVICE_H
#define _SERVICE_H

#include <windows.h>
#include "global.h"

/** Preprocessor stuff **/

#define DBT_DEVTYP_DEVICEINTERFACE 0x5
#define DEVICE_NOTIFY_SERVICE_HANDLE  0x1
#define DEVICE_NOTIFY_ALL_INTERFACE_CLASSES 0x4

/** Typedefs / Structures defintions **/

/* # Link error with my compiler, so resolving apis dynamically */
typedef HANDLE (*REGISTERDEVICENOT)(HANDLE, LPVOID, DWORD);
typedef BOOL (*UNREGISTERDEVICENOT)(HANDLE);


typedef struct
{
    DWORD dbcc_size;
    DWORD dbcc_devicetype;
    DWORD dbcc_reserved;
    GUID  dbcc_classguid;
    TCHAR dbcc_name[1];
} DEV_BROADCAST_DEVICEINTERFACE, *PDEV_BROADCAST_DEVICEINTERFACE;


/** Prototypes **/

#ifdef __cplusplus
extern "C" {
#endif

/* # Installation of the service */
DWORD servInstall(PCONFIG pConf);

/* # Start the service named 'pName' */
DWORD startServ(const char* pName);

/* # The main function of service */
VOID WINAPI servMain(DWORD dwArgc, LPTSTR *lpszArgv);

/* # Function which handles service events */
DWORD WINAPI ctrlHandler(DWORD fdwControl, DWORD evtype, PVOID evdata, PVOID Context);

#ifdef __cplusplus
}
#endif

#endif
