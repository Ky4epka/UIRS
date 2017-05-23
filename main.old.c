#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "sec_priv_interface_share.h"

void print_struct(const struct sec_priv_usersec_struct *outs)
{
	_DEBUGLOG("	%s", outs->user_name);
	_DEBUGLOG("	%d", outs->uid);
	_DEBUGLOG("	%d", outs->security_level_min);
	_DEBUGLOG("	%d", outs->security_level_max);
	_DEBUGLOG("	%s", outs->security_caps);
}

int main(int argc, char *argv[], char *envp[])
{

	_DEBUGLOG("begin...", "");
	char *libname=malloc(FILENAME_MAX);
	strcpy(libname, "sec_priv_look.so");
	void *lib=NULL;
	bool err_result=false;
	char *uname=malloc(100);
	uid_t uid=0;
printf("argc %d", argc);
	if (argc==1)
	{
		strcpy(uname, "user3");
	}
	else if (argc==2)
	{
		strcpy(uname, argv[1]);
	}
	else if (argc==3)
	{

		if (strcmp(argv[1],"uname")==0)
		{
			strcpy(uname, argv[2]);
		}
		else if (strcmp(argv[1],"uid")==0)
		{
			uid=atoi(argv[2]);
		}

	}

	struct sec_priv_userpriv_struct *outs=NULL;

	lib=dlopen(libname, RTLD_LAZY);
	err_result=false;
	char *err_string=NULL;

	if (!lib)
	{
		_ERRLOG("cannot open library '%s' - '%s'", libname, dlerror())
	}
	else
	{
		_DEBUGLOG("dlopened '%s'", libname);
		sec_priv_init_func ifunc=dlsym(lib,CFG_INIT_FUNC);

		if (ifunc==NULL)
		{
			_ERRLOG("cannot load function '%s' - '%s'", CFG_INIT_FUNC, dlerror());
		}
		else
		{
			ifunc();
		}

		sec_priv_get_last_error_func le=dlsym(lib, CFG_GET_LAST_ERROR_FUNC);

		if (le==NULL)
		{
			_ERRLOG("cannot load function '%s' - '%s'", CFG_GET_LAST_ERROR_FUNC, dlerror());
		}

		sec_priv_get_func x_func=dlsym(lib,"sec_priv_get");

		if (x_func==NULL)
		{
			_ERRLOG("cannot load function '%s' - '%s'", "sec_priv_get", dlerror());
		}
		else
		{
			_DEBUGLOG("calling... '%s' %d", "sec_priv_get", &outs);
			struct sec_priv_user_info_params_struct *params=malloc(sizeof(struct sec_priv_user_info_params_struct));
			strcpy(params->signature, COMMON_STRUCT_SIGNATURE);
			params->user_name=malloc(strlen(uname)+1);
			strcpy(params->user_name, uname);
			params->user_id=uid;

			if ((uname==NULL)||(uname=="")||(strlen(uname)==0))
			{
				err_result=x_func("sec_priv_get_by_uid", &outs, params);
			}
			else
			{
				err_result=x_func("sec_priv_get_by_uname", &outs, params);
			}

			if (le!=NULL)
			{
				err_string=le();
				_DEBUGLOG(" '%s' has returned %d %s", "sec_priv_get", err_result, le());
			}

		}

		if (err_string==NULL)
		{
			err_string="";
		}

		_DEBUGLOG("Find result: '%d' - '%s'", err_result, err_string);
		
		if (err_result)
		{
			_DEBUGLOG("Output structure: %d", outs);
			print_struct(outs);
		}

		sec_priv_free_func free_func=dlsym(lib, CFG_FREE_FUNC);

		if (free_func==NULL)
		{
			_ERRLOG("cannot load function '%s' - '%s'", CFG_FREE_FUNC, dlerror());
		}
		else
		{
			free_func();
		}

		dlclose(lib);
	}

	free(uname);
	free(libname);
	_DEBUGLOG("end", "");
	return (0);
}
