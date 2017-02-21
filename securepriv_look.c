#include "securepriv_look.h"
#include <stdio.h>

int enum_priority_list(char *uname, int uid, usersec_struct **outs)
{

	if ((uname==0)&&(uid==0))
	{
		return (SECERR_UNKNOWNERROR);
	}

	FILE *f=fopen(CFG_FILE, "r+t");

	if (f!=0)
	{
		char *libname=malloc(FILENAME_MAX);
		void *lib=0;
		getsecprivname_func *byname=0;
		getsecprivuid_func *byuid=0;
		int err=0;

		while (!feof(f))
		{
			fgets(libname,FILENAME_MAX,f);
			lib=dlopen(libname, RTLD_LAZY);
			err=SECERR_UNKNOWNERROR;

			if (!lib)
			{
				printf("cannot open library '%s'",libname);
			}
			else
			{
				
				if (uname!=0)
				{
					byname=dlsym(lib,CFG_BYNAMEFUNC);

					if (byname==0)
					{
						printf("cannot load function '%s'",CFG_BYNAMEFUNC);
					}
					else
					{
						err=*byname(uname, outs);
					}

				}
				else
				{
					byuid=dlsym(lib,CFG_BYUIDFUNC);

					if (byuid==0)
					{
						printf("cannot load function '%s'",CFG_BYNAMEFUNC);
					}
					else
					{
						err=*byuid(uid, outs);
					}

				}

				dlclose(lib);
			}

			if ((err==SECERR_SUCCESS)||(err==SECERR_OUTOFMEM)||(err==SECERR_UNKNOWNERROR))
			{
				fclose(f);
				free(libname);
				return (err);
			}

		}

		free(libname);
		fclose(f);
	}
	else
	{
		return (SECERR_CFGREADERROR);
	}

}

int getsecprivname(char *uname, usersec_struct **outs)
{

	if (outs==0)
	{
		return (SECERR_NULLSTRUCTPTR);
	}
	else if (uname==0)
	{
		return (SECERR_EMPTYUSERNAME);
	}

	return (enum_priority_list(uname, 0, outs));
}

int getsecprivid(int uid, usersec_struct **outs)
{
	
	if (outs==0)
	{
		return (SECERR_NULLSTRUCTPTR);
	}
	else if (uid==0)
	{
		return (SECERR_EMPTYUID);
	}

	return (enum_priority_list(0, uid, outs));
}