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
#include "ini.h"
#include "debug.h"

BOOL initConfigurationStructure(PCONFIG pConf)
{
    #define MAX_CHAR 255
    DWORD status = 0, i = 0, sizeStr = 0;
    CHAR oneRule = 0, tmp[MAX_CHAR] = {0}, *tmp2 = NULL, *token = NULL, *section[][2] = {
        {SERVICE_CONFIGURATION_SECTION, "name"},
        {SERVICE_CONFIGURATION_SECTION, "desc"},
        {MAIN_CONFIGURATION_SECTION   , "output_dir"},
        {MAIN_CONFIGURATION_SECTION   , "max_size"},
        {MAIN_CONFIGURATION_SECTION   , "recurse_max"},
        {NULL, NULL}
    },
    **ptrStr[] = {
        &pConf->serviceName, &pConf->serviceDesc, &pConf->outputPath, &pConf->max_size, &pConf->recurse_max
    },
    *defaultValues[] = {
        DEFAULT_SERVICE_NAME,
        DEFAULT_SERVICE_DESC,
        DEFAULT_OUTPUT_DIR,
        DEFAULT_FILE_MAX_SIZE,
        DEFAULT_RECURSE_MAX_LEVEL
    };

    ZeroMemory(pConf, sizeof(CONFIG));
    DEBUGMSG("| Parsing INI configuration file..\n");

    for(; section[i][0] != NULL && section[i][1] != NULL; ++i)
    {
        status = GetPrivateProfileString(section[i][0],
            section[i][1],
            defaultValues[i],
            tmp,
            MAX_CHAR,
            DEFAULT_CONFIG_FILE
        );

        if(strcmp(tmp, "") == 0)
        {
            DEBUGMSG("'%s.%s'  is equal to \"\", this is not allowed.\n", section[i][0], section[i][1]);
            goto error;
        }

        sizeStr = strlen(tmp) + 1;
        *(ptrStr[i]) = (char*)malloc(sizeof(char) * sizeStr);
        if(*(ptrStr[i]) == NULL)
        {
            DEBUGMSG("- Can't allocate memory.\n");
            goto error;
        }

        ZeroMemory(*(ptrStr[i]), sizeStr);
        memcpy(*(ptrStr[i]), tmp, sizeStr - 1);
        ZeroMemory(tmp, MAX_CHAR);
    }

    tmp2 = (char*)pConf->max_size;
    pConf->max_size = atoi(tmp2);
    free(tmp2);

    tmp2 = (char*)pConf->recurse_max;
    pConf->recurse_max = atoi(tmp2);
    free(tmp2);

    if(pConf->outputPath[strlen(pConf->outputPath) - 1] != '\\')
    {
        DEBUGMSG("Please append the character '\\' at the end of %s.output_dir parameter.\n", MAIN_CONFIGURATION_SECTION);
        goto error;
    }

    /* patterns */
    status = GetPrivateProfileString(MAIN_CONFIGURATION_SECTION,
        "patterns",
        DEFAULT_PATTERN,
        tmp,
        MAX_CHAR,
        DEFAULT_CONFIG_FILE
    );

    pConf->patterns = (char**)malloc(sizeof(char*));
    if(pConf->patterns == NULL)
    {
        DEBUGMSG("- Can't allocate memory for pattern table.\n");
        goto error;
    }

    token = strtok(tmp, PATTERN_SEPARATOR);
    while(token != NULL)
    {
        if(oneRule == 0)
            oneRule = 1;

        sizeStr = strlen(token) + 1;

        pConf->patterns[pConf->nbPattern] = (char*)malloc(sizeof(char) * sizeStr);
        if(pConf->patterns[pConf->nbPattern] == NULL)
        {
            DEBUGMSG("- Can't allocate memory for the pattern rule.\n");
            goto error;
        }

        ZeroMemory(pConf->patterns[pConf->nbPattern], sizeStr);
        memcpy(pConf->patterns[pConf->nbPattern], token, sizeStr - 1);

        pConf->nbPattern++;
        token = strtok(NULL, PATTERN_SEPARATOR);
    }

    if(oneRule == 0)
    {
        sizeStr = strlen(tmp) + 1;

        if(strcmp(tmp, "") == 0)
        {
            DEBUGMSG("'main_configuration.patterns'  is equal to \"\", this is not allowed.\n");
            goto error;
        }
        else if(tmp[0] == ';')
        {
            DEBUGMSG("'main_configuration.patterns'  contains only the separator.\n");
            goto error;
        }
        else
        {
            pConf->patterns[0] = (char*)malloc(sizeof(char) * sizeStr);
            if(pConf->patterns[0] == NULL)
            {
                DEBUGMSG("- Can't allocate memory for the pattern rule.\n");
                goto error;
            }

            pConf->nbPattern = 1;

            ZeroMemory(pConf->patterns[0], sizeStr);
            memcpy(pConf->patterns[0], tmp, sizeStr - 1);
        }
    }

    return TRUE;

    error:
    if(pConf->serviceName != NULL)
        free(pConf->serviceName);

    if(pConf->serviceDesc != NULL);
        free(pConf->serviceDesc);

    if(pConf->outputPath != NULL);
        free(pConf->outputPath);

    if(pConf->nbPattern > 0)
    {
        for(; i < pConf->nbPattern; ++i)
            free(pConf->patterns[i]);
        free(pConf->patterns);
    }

    return FALSE;
}

BOOL createConfigurationFile()
{
    FILE *fp = NULL;

    fp = fopen("./config.ini", "w");
    if(fp == NULL)
    {
        DEBUGMSG("Can't write config.ini file.\n");
        return FALSE;
    }

    fprintf(fp, "[%s]\n", SERVICE_CONFIGURATION_SECTION);
    fprintf(fp, "name=\"%s\"\n", DEFAULT_SERVICE_NAME);
    fprintf(fp, "desc=\"%s\"\n\n", DEFAULT_SERVICE_DESC);

    fprintf(fp, "[%s]\n", MAIN_CONFIGURATION_SECTION);
    fprintf(fp, "patterns=\"%s\"\n", DEFAULT_PATTERN);
    fprintf(fp, "max_size=%u\n", DEFAULT_FILE_MAX_SIZE);
    fprintf(fp, "recurse_max=%u\n", DEFAULT_RECURSE_MAX_LEVEL);
    fprintf(fp, "output_dir=\"%s\"\n", DEFAULT_OUTPUT_DIR);

    fclose(fp);
    return TRUE;
}
