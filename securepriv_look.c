#include "securetypes.h"
#include "securepriv_look.h"
#include <stdio.h>
#include <dlfcn.h>

bool enum_priority_list(const char *uname, const uid_t uid, struct sec_priv_usersec_struct **outs)
{

	if ((uname==NULL)&&(uid==0))
	{
		_ERRLOG("has returned 'SECERR_UNKNOWN_ERROR'", "")
		sec_priv_set_last_error(SECERR_UNKNOWN_ERROR);
		return (false);
	}

	FILE *f=fopen(CFG_FILE, "r+t");

	if (f==NULL)
	{
		_ERRLOG("has returned 'SECERR_CFG_READ_ERROR'", "")
		sec_priv_set_last_error(SECERR_CFG_READ_ERROR);
		return (false);
	}

	_DEBUGLOG("'%s' opened ", CFG_FILE);

	char *libname=malloc(FILENAME_MAX);

	if (libname==NULL)
	{
		_ERRLOG("has returned 'SECERR_OUT_OF_MEM'", "")
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		fclose(f);
		return (false);
	}

	void *lib=NULL;
	sec_priv_get_name_func byname=0;
	sec_priv_get_uid_func byuid=0;
	bool err_result=false;
	char *err_string=NULL;

	while (!feof(f))
	{
		fgets(libname,FILENAME_MAX,f);
		
		if (strlen(libname)>0)
		{
			libname[strlen(libname)-1]=0;
		}

		lib=dlopen(libname, RTLD_LAZY);

		if ((!lib)||(strlen(libname)==0))
		{
			_ERRLOG("cannot open library '%s' - '%s'", libname, dlerror());
			continue;
		}

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
				err_result=byname(uname, outs);

				if (le!=NULL)
				{
					err_string=le();
					sec_priv_set_last_error(err_string);
					_DEBUGLOG(" '%s' has returned %d %s %d", CFG_BY_NAME_FUNC, err_result, le(), *outs);
				}

			}

		}
		else
		{
			byuid=dlsym(lib,CFG_BY_UID_FUNC);

			if (byuid==NULL)
			{
				_ERRLOG("cannot load function '%s' - '%s'", CFG_BY_UID_FUNC, dlerror());
			}
			else
			{
				_DEBUGLOG("calling... '%s'", CFG_BY_NAME_FUNC);
				err_result=byuid(uid, outs);

				if (le!=NULL)
				{
					err_string=le();
					sec_priv_set_last_error(err_string);
					_DEBUGLOG(" '%s' has returned %d %s", CFG_BY_NAME_FUNC, err_result, le());
				}

			}

		}

		dlclose(lib);

		if (err_result)
		{
			fclose(f);
			free(libname);
			return (err_result);
		}

	}

	return (err_result);
	free(libname);
	fclose(f);
}

void sec_priv_init()
{
	sec_priv_dl_init();
	sec_priv_set_enum_db_func(enum_priority_list);
	_DEBUGLOG("'securepriv_look.so' initialized", "")
}
