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
#ifndef _INI_H
#define _INI_H

#include <windows.h>
#include "global.h"

#define DEFAULT_SERVICE_NAME "j0llyDmpr"
#define DEFAULT_SERVICE_DESC "j0llyDmpr by 0vercl0k (0vercl0k.tuxfamily.org)."
#define DEFAULT_CONFIG_FILE "./config.ini"
#define DEFAULT_OUTPUT_DIR "C:\\DUMPED\\"
#define DEFAULT_PATTERN ".doc"
#define DEFAULT_FILE_MAX_SIZE 1000U
#define DEFAULT_RECURSE_MAX_LEVEL 10U

#define PATTERN_SEPARATOR ";"

#define SERVICE_CONFIGURATION_SECTION "service_configuration"
#define MAIN_CONFIGURATION_SECTION "main_configuration"

#ifdef __cplusplus
extern "C" {
#endif

/* # Parsing of configuration file, and creating configuration structure */
BOOL initConfigurationStructure(PCONFIG pConf);

/* # Create the default configuration file */
BOOL createConfigurationFile();

#ifdef __cplusplus
}
#endif

#endif
