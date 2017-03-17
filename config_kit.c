#include "config_kit.h"

extern struct config_struct *cfg_struct;

struct config_struct *cfg_struct=NULL;

void display_module_config(const struct module_struct* ms)
{
	printf("\t--MODULE CONFIG--\n");
	printf("\tSIGNATURE: %s\n", ms->signature);
	printf("\tNAME: %s\n", ms->name);
	printf("\tLIB_NAME: %s\n", ms->lib_name);
	printf("\tPARAM COUNT: %d\n", ms->param_count);
	printf("\tPARAMS:\n");

	int i=0;
	while (i<ms->param_count)
	{
		printf("\t\t %s\n", ms->params[i]);
		i++;
	}

	printf("\tFUNCTION COUNT: %d\n", ms->export_functions_count);

	i=0;
	while (i<ms->export_functions_count)
	{
		printf("\t\t FUNCTION: %s\n", ms->export_functions[i]);
		i++;
	}

}

void display_config(struct config_struct* cs)
{

	if (cs==NULL)
	{
		cs=cfg_struct;
	}

	printf("\n--CONFIG--\n");
	printf("CONFIG SIGNATURE: %s\n", cs->signature);
	printf("TIMEOUT: %d\n", cs->time_out);
	printf("MODULES: \n");

	struct module_struct *ms=cs->first_module;

	while (ms!=NULL)
	{
		display_module_config(ms);
		ms=ms->next_module;
	}

}

void enum_dirs(const char* start_dir, char ***_dirs, int32_t *dir_count)
{
	char path_name[PATH_MAX];
	int32_t realloc_quota=50;
	int32_t capacity=realloc_quota;
    	DIR *dir = opendir(start_dir);
	*dir_count=0;
	*_dirs=NULL;
	int32_t dcount=0;
	char **dirs=(char**)malloc(capacity*sizeof(char*));
	printf("enum_dirs\n");

	if(dir)
	{
		struct dirent *ent;

		while ((ent = readdir(dir)) != NULL)
		{
			struct stat st;
		
			if ((strncmp(ent->d_name, ".", PATH_MAX)==0) ||
				(strncmp(ent->d_name, "..", PATH_MAX)==0))
			{
				continue;
			}

			strncpy(path_name, start_dir, PATH_MAX);
			strncat(path_name, "/", PATH_MAX);
			strncat(path_name, ent->d_name, PATH_MAX);

			if (stat(path_name, &st)!=0)
			{
				continue;
			}

			if (S_ISDIR(st.st_mode)) {
				dirs[dcount]=malloc(strlen(ent->d_name)+1);
				strcpy(dirs[dcount], ent->d_name);
				printf("	DIRS: %s\n", dirs[dcount]);
				dcount++;
				capacity++;

				if (capacity%realloc_quota==0)
				{
					capacity+=realloc_quota;
					dirs=(char**)realloc(dirs, capacity*sizeof(char*));
				}

			}

		}
	}
	else
	{
		fprintf(stderr, "Error opening directory\n");
	}

	*_dirs=dirs;
	*dir_count=dcount;
	closedir(dir);
}

char *strupr(char *str)
{

	int i=0;
	while (i<strlen(str)) 
	{
		str[i] = toupper(str[i]);
		i++;
	}

	return (str);
}

char *str_del_endline(char *str)
{
	int i=strlen(str);

	while (i>=0)
	{

		if (str[i]==10)
		{
			str[i]=0;
		}

		i--;
	}

	return (str);
}

char *chk_dir( char **dirs, uint32_t dir_count, const char *dir)
{
	char udir[PATH_MAX];
	char cdir[PATH_MAX];

	strcpy(udir, dir);
	strupr(udir);

	int i=0;
	while (i<dir_count)
	{
		strcpy(cdir,dirs[i]);
		strupr(cdir);
//printf("UDIR '%s' CDIR '%s'\n", udir, cdir);

		if (strcmp(cdir,udir)==0)
		{
			return (dirs[i]);
		}

		i++;
	}

	return (NULL);
}


bool contain_export_function(const char* func)
{

	if (cfg_struct==NULL)
	{
		return (false);
	}

	struct module_struct *ms=cfg_struct->first_module;
	int i=0;

	while (ms!=NULL)
	{

		i=0;
		while (i<ms->export_functions_count)
		{

			if (strcmp(func, ms->export_functions[i])==0)
			{
				return (true);
			}

			i++;
		}

		ms=ms->next_module;
	}

	return (false);
}

void load_module_config(struct module_struct *ms, const char *module_dir)
{
	char path[PATH_MAX];
	strncpy(path, module_dir, PATH_MAX);
	strncat(path, "/", PATH_MAX);
	strncat(path, MODULE_CONFIG_FILE_NAME, PATH_MAX);
	FILE *cfg=fopen(path, "r+t");

	if (cfg==NULL)
	{
		return;
	}

	fgets(ms->signature, MODULE_CONFIG_SIGNATURE_LEN, cfg);

	if (strncmp(ms->signature, MODULE_CONFIG_SIGNATURE, MODULE_CONFIG_SIGNATURE_LEN)!=0)
	{
		fclose(cfg);
		return;
	}

	char line[1024];
	bool functions=false;
	ms->export_functions_count=0;
	ms->export_functions=0;
	char *buf=NULL;

	while (!feof(cfg))
	{

		if (!fgets(line, 1024, cfg))
		{
			continue;
		}

		str_del_endline(line);
		buf=strstr(line, MODULE_CONFIG_LIB_NAME_DEFINE);

		if (buf!=NULL)
		{
			buf+=strlen(MODULE_CONFIG_LIB_NAME_DEFINE);
			buf=strtok(buf, CONFIG_PARAM_DELIMITERS);
			strncpy(ms->lib_name, module_dir, PATH_MAX);
			strncat(ms->lib_name, "/", PATH_MAX);
			strncat(ms->lib_name, buf, PATH_MAX);
printf("DEBUG: %s\n", ms->lib_name);
		}

		if (functions)
		{
			char *fn=strtok(line, CONFIG_PARAM_DELIMITERS);

			if ((fn!=NULL)&&(strlen(fn)>0))
			{
				ms->export_functions_count++;
				ms->export_functions=(char**)realloc(ms->export_functions, ms->export_functions_count*sizeof(char*));
				char *fname=malloc(strlen(fn)+1);
				strcpy(fname, fn);
				ms->export_functions[ms->export_functions_count-1]=fname;
			}

		}

		if ((!functions)&&(strstr(line, MODULE_CONFIG_FUNCTIONS_GROUP)!=NULL))
		{
			functions=true;
		}

	}

}

void load_config()
{
	FILE *cfg=fopen(CONFIG_FILE_NAME, "r+t");

	if (cfg==NULL)
	{
		printf("cfgnull\n");
		return;
	}

	if (cfg_struct==NULL)
	{
		cfg_struct=malloc(sizeof(struct config_struct));
		memset(cfg_struct, 0, sizeof(struct config_struct));
		cfg_struct->first_module=NULL;
		cfg_struct->last_module=NULL;
	}

	fgets(cfg_struct->signature, CONFIG_SIGNATURE_LEN, cfg);

	if (strncmp(cfg_struct->signature, CONFIG_SIGNATURE, CONFIG_SIGNATURE_LEN)!=0)
	{
		printf("sigmissing %s\n", cfg_struct->signature);
		fclose(cfg);
		return;
	}

	char **dirs;
	int32_t dir_count=0;
	enum_dirs(".", &dirs, &dir_count);

	char line[1024];
	struct module_struct *ms;
	cfg_struct->first_module=NULL;
	cfg_struct->last_module=NULL;
	bool modules=false;

	while (!feof(cfg))
	{

		if (!fgets(line, 1024, cfg))
		{
			continue;
		}

		str_del_endline(line);

		if (strstr(line, CONFIG_TIME_OUT_STRING)!=NULL)
		{
			char *val=strstr(line, CONFIG_VALUE_ASSIGN_DELIMITER)+strlen(CONFIG_VALUE_ASSIGN_DELIMITER);
			cfg_struct->time_out=atoi(val);
			modules=false;
		}

		if ((modules)&&(strlen(line)>0))
		{
			char *tok=strtok(line, CONFIG_PARAM_DELIMITERS);
			int counter=0;
			bool load_mconfig=false;
			
			while (tok!=NULL)
			{

				if (counter==0)
				{
printf("tok: %s\n",tok);
					char *dir_name=chk_dir(dirs, dir_count, tok);

					if (dir_name==NULL)
					{
						printf("Module path not found\n");
						break;
					}

					ms=malloc(sizeof(struct module_struct));
					memset(ms, 0, sizeof(struct module_struct));
					ms->next_module=NULL;

					if (cfg_struct->first_module==NULL)
					{
						cfg_struct->first_module=ms;
					}
					else
					{
						cfg_struct->last_module->next_module=ms;
					}

					strncpy(ms->name, tok, CONFIG_MODULE_NAME_MAX);
					cfg_struct->last_module=ms;
					load_mconfig=true;
				}
				else if (counter>CONFIG_MODULE_PARAM_COUNT_MAX)
				{
					break;
				}
				else
				{
					strncpy(ms->params[counter-1], tok, CONFIG_MODULE_PARAM_STR_MAX);
				}

				counter++;
				ms->param_count=counter-1;
				tok=strtok(NULL, CONFIG_PARAM_DELIMITERS);
			}

			if (load_mconfig)
			{				
				load_module_config(ms, ms->name);
			}			


		}

		if ((!modules)&&(strstr(line, CONFIG_PRIORITY_MODULES_GROUP)!=NULL))
		{
			modules=true;
		}

	}

	int i=0;
	while (i<dir_count)
	{
		free(dirs[i]);
		i++;
	}

	fclose(cfg);
}

