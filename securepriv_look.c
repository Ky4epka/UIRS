#include "securetypes.h"
#include "securepriv_look.h"
#include <stdio.h>
#include <dlfcn.h>
#include "config_kit.h"

struct sec_priv_usersec_struct *last_struct=NULL;

bool enum_priority_list(const char *func_name, struct sec_priv_usersec_struct **outs, const void **params)
{

	if (cfg_struct==NULL)
	{
		sec_priv_set_last_error(SECERR_CFG_READ_ERROR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	_DEBUGLOG("'%s' opened ", CFG_FILE);

	char *libname=malloc(FILENAME_MAX);

	if (libname==NULL)
	{
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	void *lib=NULL;
	sec_priv_getX_func get_func=0;
	bool err_result=false;
	char *err_string=NULL;

	struct module_struct *ms=cfg_struct->first_module;
	while (ms!=NULL)
	{
		strcpy(libname, ms->lib_name);
		lib=dlopen(libname, RTLD_LAZY);

		if ((!lib)||(strlen(libname)==0))
		{
			_ERRLOG("cannot open library '%s' - '%s'", libname, dlerror());
			ms=ms->next_module;
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
		
		get_func=dlsym(lib,func_name);

		if (get_func==NULL)
		{
			_ERRLOG("cannot load function '%s' - '%s'", func_name, dlerror());
		}
		else
		{
			_DEBUGLOG("calling... '%s' %d %d", func_name, outs, params);
			err_result=get_func(outs, params);
			_DEBUGLOG("end calling... '%s'", func_name);

			if (le!=NULL)
			{
				err_string=le();
				sec_priv_set_last_error(err_string);
				_DEBUGLOG(" '%s' has returned %d %s %d", func_name, err_result, le(), 0);
			}

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

		if (err_result)
		{
			last_struct=*outs;
			free(libname);
			return (err_result);
		}

		ms=ms->next_module;
	}

	return (err_result);
	free(libname);
}


bool sec_priv_get(const char *func_name, struct sec_priv_usersec_struct **outs, const void *params)
{

	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}
	else if (func_name==NULL)
	{
		sec_priv_set_last_error(SECERR_EMPTY_FUNC_NAME);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	if (!contain_export_function(func_name))
	{
		sec_priv_set_last_error(SECERR_EXPORT_FUNC_NOT_FOUND);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	return (enum_priority_list(func_name, outs, params));
}

void sec_priv_init()
{
	load_config();
	display_config(NULL);
	sec_priv_dl_init();
	_DEBUGLOG("'securepriv_look.so' initialized", "")
}

void sec_priv_free()
{

	if (last_struct!=NULL)
	{

		if (last_struct->user_name!=NULL)
		{
			free(last_struct->user_name);
		}

		free(last_struct);
		last_struct=NULL;
	}

}
