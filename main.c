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
#include <windows.h>
#include <string.h>

#include "main.h"
#include "debug.h"
#include "service.h"
#include "color_shell.h"
#include "ini.h"
#include "global.h"

/**

    TODO : - x64 compat
**/

/** Definitions **/

int main(int argc, char* argv[])
{
    SC_HANDLE hServMngr = NULL, hServ = NULL;
    DWORD ret = EXIT_SUCCESS, i = 0;
    FILE *fp = NULL;
    BOOL status = TRUE, serviceIsCreated = FALSE;
    SERVICE_TABLE_ENTRY servTableEntry[] =
        {
            {
                NULL,
                servMain
            },

            {
                NULL,
                NULL
            }

        };

    InitColor();
    DisplayProjectName();

    printf("* Debug-manager initialization.. ");
    initDbgManager();
    DisplayOk();

    printf("* Testing if the configuration file exists.. ");
    if((fp = fopen(DEFAULT_CONFIG_FILE, "r")) == NULL)
    {
        DisplayKo();
        printf("* Trying to create the default file.. ");
        if(createConfigurationFile() == FALSE)
        {
            DisplayKo();
            ret = EXIT_FAILURE;
            goto clean;
        }
        else
            DisplayOk();
    }
    else
        DisplayOk()

    printf("* Parsing file configuration.. ");
    if(initConfigurationStructure(&globalConfiguration) == TRUE)
        DisplayOk()
    else
    {
        DisplayKo();
        ret = EXIT_FAILURE;
        goto clean;
    }

    servTableEntry[0].lpServiceName = globalConfiguration.serviceName;
    printf("\t - Service Name   : '%s'\n", globalConfiguration.serviceName);
    printf("\t - Service Desc   : '%s'\n", globalConfiguration.serviceDesc);
    printf("\t - Output Dir     : '%s'\n", globalConfiguration.outputPath);
    printf("\t - Number pattern :  %u \n", globalConfiguration.nbPattern);
    for(; i < globalConfiguration.nbPattern; ++i)
        printf("\t\t - Patterns       : '%s'\n", globalConfiguration.patterns[i]);
    printf("\t - File max size  :  %u \n", globalConfiguration.max_size);
    printf("\t - Recursion lvls :  %u \n", globalConfiguration.recurse_max);

    if(OpenService(OpenSCManager(NULL, NULL, GENERIC_EXECUTE), globalConfiguration.serviceName, GENERIC_EXECUTE) == NULL)
    {
        DEBUGMSG("- Service not registered, registering.. ");
        printf("* Trying to register the service.. ");
        if(servInstall(&globalConfiguration))
            DisplayOk()
        else
        {
            DisplayKo();
            printf("\t /!\\ Cannot register the service -> Service already registered ?.\n\n");
            ERRORMSG("Fail to register the service");
        }

        DEBUGMSG("- Starting the service.. ");
        printf("* Trying to start the service.. ");

        if(!startServ(globalConfiguration.serviceName))
        {
            DisplayKo();
            printf("\t /!\\ Cannot launch the service.\n\n");
            ERRORMSG("Fail to start the service");

            ret = EXIT_FAILURE;
            goto clean;
        }

        DisplayOk();
    }
    else
    {
        DEBUGMSG("- Service already registered.\n");
        printf("* Service already registered, let me start it.. ");
        status = StartServiceCtrlDispatcher(servTableEntry);
        if(status == FALSE)
        {
            /* Ici, on tente de lancer le service en passant par le binaire */
            if(GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
            {
                if(!startServ(globalConfiguration.serviceName))
                {
                    DisplayKo();
                    printf("\t /!\\ Cannot launch the service.\n\n");
                    ERRORMSG("Fail to start the service");

                    ret = EXIT_FAILURE;
                    goto clean;
                }

                DisplayOk();
            }
            else
            {
                DisplayKo();
                printf("\t /!\\ Cannot start the service.\n\n");
                ERRORMSG("Fail to connect main thread to the service manager");

                ret = EXIT_FAILURE;
                goto clean;
            }
        }
    }

    clean:

    CloseDbgManager();
    closeColor();

    if(hServ != NULL)
        CloseHandle(hServ);

    if(hServMngr != NULL)
        CloseServiceHandle(hServMngr);

    getchar();
    return ret;
}


