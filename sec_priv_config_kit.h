#ifndef __CONFIG_KIT_a9c714415de8ff9b0d7aeb9152cfebc843cb6b50
#define __CONFIG_KIT_a9c714415de8ff9b0d7aeb9152cfebc843cb6b50


#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>

/*
	Config file format:
	
		1 line: CONFIG_SIGNATURE
	
		N line: CONFIG_TIME_OUT_STRING (not necessary)

		N line: CONFIG_PRIORITY_MODULES_GROUP (begin modules list)

			Module line: <Module name (ANSI)> <params (delims CONFIG_PARAM_DELIMITERS)>
				#Note: Module name must be equal name of module path (Register case not sensitive)

	Module config file format:

		1 line: MODULE_CONFIG_SIGNATURE
		
		N line: LIB_NAME <Library file name>

		N line: MODULE_CONFIG_FUNCTIONS_GROUP

			Function line: <Function name (ANSI)>
*/


#define CONFIG_FILE_NAME			"/usr/lib/_sec_priv/settings.cfg"
#define CONFIG_PARAM_DELIMITERS			" \t"
#define CONFIG_VALUE_ASSIGN_DELIMITER		"="

#define CONFIG_SIGNATURE_LEN			6
#define CONFIG_SIGNATURE			"[CFG]"

#define CONFIG_TIME_OUT_DEFAULT			10000
#define CONFIG_TIME_OUT_STRING			"TIME_OUT"
#define CONFIG_MODULES_PATH_DEFAULT		"/usr/lib/_sec_priv"
#define CONFIG_MODULES_PATH_STRING		"MODULES_PATH"

#define CONFIG_PRIORITY_MODULES_GROUP		"[MODULES]"
#define CONFIG_MODULE_NAME_MAX			256
#define CONFIG_MODULE_LIBRARY_NAME_MAX		PATH_MAX
#define CONFIG_MODULE_PARAM_STR_MAX		50
#define CONFIG_MODULE_PARAM_COUNT_MAX		10

#define MODULE_CONFIG_FILE_NAME			"module.cfg"
#define MODULE_CONFIG_SIGNATURE_LEN		13
#define MODULE_CONFIG_SIGNATURE			"[MODULE_CFG]"
#define MODULE_CONFIG_LIB_NAME_DEFINE		"LIB_NAME"
#define MODULE_CONFIG_FUNCTIONS_GROUP		"FUNCTIONS"
#define MODULE_CONFIG_FUNCTION_NAME_MAX		50


struct module_struct
{
	char signature[MODULE_CONFIG_SIGNATURE_LEN+1];
	char name[CONFIG_MODULE_NAME_MAX+1];
	char lib_name[CONFIG_MODULE_LIBRARY_NAME_MAX+1];
	char params[CONFIG_MODULE_PARAM_COUNT_MAX][CONFIG_MODULE_PARAM_STR_MAX];
	char param_count;

	char **export_functions;
	int32_t export_functions_count;

	struct module_struct *next_module;
};

struct config_struct
{
	char signature[CONFIG_SIGNATURE_LEN+1];
	uint32_t time_out;
	char modules_path[1024];
	struct module_struct *first_module;
	struct module_struct *last_module;
};



void load_config();
void load_module_config(struct module_struct *ms, const char *module_dir);
void display_config(struct config_struct* cs);
void display_module_config(const struct module_struct* ms);
bool contain_export_function(const char* func);

extern struct config_struct *cfg_struct;

#endif
