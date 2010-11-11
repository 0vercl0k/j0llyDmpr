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
#include "usb.h"
#include "debug.h"

/** Definitions **/

UCHAR GetLetterOfNewVolume(const DWORD precVal, const DWORD newVal)
{
	#define GetBit(x, nb) ((x >> nb) & 0x1)
	DWORD i = 0;
	for(; i < 32; ++i)
	{
		if(GetBit(precVal, i) != GetBit(newVal, i))
			return 'A' + i;
	}

	return 0;
}

UCHAR DumpAndSearchInteresstingFiles(const PUCHAR pVol, const DWORD lvl, PCONFIG pConf)
{
    #define RECURSE_MAX 10

    WIN32_FIND_DATA findData = {0};
    PUCHAR researchPatternPath = NULL,
        directoryPath = NULL,
        filePathToCopy = NULL,
        filePathCopied = NULL;
    HANDLE hFind = 0;
    DWORD sizeStr = strlen(pVol) + 1 + 1,
        sizeStrDirectory = 0,
        sizeFilePathToCopy = 0,
        sizeFilePathCopied = 0,
        ret = 1,
        status = 0;

    researchPatternPath = (PUCHAR)malloc(sizeof(UCHAR) * sizeStr);
    if(researchPatternPath == NULL)
    {
        ret = 0;
        return ret;
    }

    ZeroMemory(researchPatternPath, sizeStr);
    strncpy(researchPatternPath, pVol, strlen(pVol));
    strncat(researchPatternPath, "*", 1);

    hFind = FindFirstFile(
        researchPatternPath,
        &findData
    );

    if(hFind == INVALID_HANDLE_VALUE)
    {
        free(researchPatternPath);
        ret = 0;
        return ret;
    }

    do
    {
        if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
             if(isAnInteresstingFile(findData.cFileName, ((findData.nFileSizeHigh << 32) | findData.nFileSizeLow), pConf) == 1)
             {
                sizeFilePathToCopy = strlen(pVol) + strlen(findData.cFileName) + 1;
                sizeFilePathCopied = strlen(pConf->outputPath) + strlen(findData.cFileName) + 1;

                filePathToCopy = (PUCHAR)malloc(sizeof(char) * sizeFilePathToCopy);
                filePathCopied = (PUCHAR)malloc(sizeof(char) * sizeFilePathCopied);

                if(filePathToCopy == NULL || filePathCopied == NULL)
                {
                    ret = 0;
                    goto clean;
                }

                ZeroMemory(filePathToCopy, sizeFilePathToCopy);
                ZeroMemory(filePathCopied, sizeFilePathCopied);

                strncpy(filePathToCopy, pVol, strlen(pVol));
                strncat(filePathToCopy, findData.cFileName, strlen(findData.cFileName));

                strncpy(filePathCopied, pConf->outputPath, strlen(pConf->outputPath));
                strncat(filePathCopied, findData.cFileName, strlen(findData.cFileName));

                DEBUGMSG("Copying '%s' to '%s'..\n", filePathToCopy, filePathCopied);

                status = CopyFile(
                    filePathToCopy,
                    filePathCopied,
                    TRUE
                );

                if(status != 0)
                {
                    status = GetFileAttributes(
                        filePathCopied
                    );

                    if(status != INVALID_FILE_ATTRIBUTES && (status & FILE_ATTRIBUTE_HIDDEN) != 0)
                    {
                        SetFileAttributes(
                            filePathCopied,
                            status & (~FILE_ATTRIBUTE_HIDDEN)
                        );
                    }
                }

                free(filePathToCopy);
                free(filePathCopied);

                filePathToCopy = filePathCopied = NULL;
             }
        }
        else
        {
            if(lvl != (RECURSE_MAX - 1) && strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
            {
                sizeStrDirectory = strlen(pVol) + strlen(findData.cFileName) + 1 + 1;
                directoryPath = (PUCHAR)malloc(sizeof(char) * sizeStrDirectory);
                if(directoryPath == NULL)
                {
                    ret = 0;
                    goto clean;
                }

                ZeroMemory(directoryPath, sizeStrDirectory);
                strncpy(directoryPath, pVol, strlen(pVol));
                strncat(directoryPath, findData.cFileName, strlen(findData.cFileName));
                strncat(directoryPath, "\\", 1);

                DumpAndSearchInteresstingFiles(directoryPath, lvl+1, pConf);
                free(directoryPath);
            }
        }

    }while(FindNextFile(hFind, &findData) != 0);

    clean:

    if(filePathToCopy != NULL)
        free(filePathToCopy);

    if(filePathCopied != NULL)
        free(filePathCopied);

    free(researchPatternPath);
    FindClose(hFind);
    return ret;
}

UCHAR isAnInteresstingFile(const PUCHAR file, const unsigned long long fileSize, PCONFIG pConf)
{
    DWORD i = 0;

    for(; i < pConf->nbPattern; ++i)
    {
        if(strstr(file, pConf->patterns[i]) != NULL && fileSize <= pConf->max_size)
            return 1;
    }

    return 0;
}

VOID initUsbStuff(const char* outpath)
{
    CreateDirectory(outpath,
        NULL
    );
}
