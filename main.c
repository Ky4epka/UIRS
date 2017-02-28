#include "securetypes.h"
#include "securepriv_look.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

void print_struct(const struct sec_priv_usersec_struct *outs)
{
	_DEBUGLOG("	%s", outs->user_name);
	_DEBUGLOG("	%d", outs->uid);
	_DEBUGLOG("	%d", outs->security_level);
	_DEBUGLOG("	%d", outs->security_category);
}

int main(int argc, char *argv[], char *envp[])
{
	_DEBUGLOG("begin...", "");
	char *libname=malloc(FILENAME_MAX);
	strcpy(libname, "securepriv_look.so");
	void *lib=NULL;
	sec_priv_get_name_func byname=0;
	sec_priv_get_uid_func byuid=0;
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

	struct sec_priv_userpriv_struct *outs=0;

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

		if ((uname!=NULL)&&(strlen(uname)>0))
		{
			byname=dlsym(lib,CFG_BY_NAME_FUNC);

			if (byname==NULL)
			{
				_ERRLOG("cannot load function '%s' - '%s'", CFG_BY_NAME_FUNC, dlerror());
			}
			else
			{
				_DEBUGLOG("calling... '%s'", CFG_BY_NAME_FUNC);
				err_result=byname(uname, &outs);

				if (le!=NULL)
				{
					err_string=le();
					_DEBUGLOG(" '%s' has returned %d %s", CFG_BY_NAME_FUNC, err_result, le());
				}

			}

		}
		else
		{
			byuid=dlsym(lib,CFG_BY_UID_FUNC);

			if (byuid==0)
			{
				_ERRLOG("cannot load function '%s' - '%s'", CFG_BY_UID_FUNC, dlerror());
			}
			else
			{
				_DEBUGLOG("calling... '%s'", CFG_BY_NAME_FUNC);
				err_result=byuid(uid, &outs);

				if (le!=NULL)
				{
					err_string=le();
					_DEBUGLOG(" '%s' has returned %d %s", CFG_BY_NAME_FUNC, err_result, le());
				}

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

		dlclose(lib);
	}

	free(uname);
	free(libname);
	_DEBUGLOG("end", "");
	return (0);
}
