#include "securetypes.h"
#include "securepriv_look.h"
#include <stdio.h>
#include <dlfcn.h>

int enum_priority_list(char *uname, int uid, struct usersec_struct **outs)
{

	if ((uname==0)&&(uid==0))
	{
		return (SECERR_UNKNOWNERROR);
	}

	FILE *f=fopen(CFG_FILE, "r+t");

	if (f!=0)
	{
		printf("debug: file '%s' opened\n", CFG_FILE);
		char *libname=malloc(FILENAME_MAX);
		void *lib=0;
		getsecprivname_func byname=0;
		getsecprivuid_func byuid=0;
		int err=0;

		while (!feof(f))
		{
			fgets(libname,FILENAME_MAX,f);
			
			if (strlen(libname)>0)
			{
				libname[strlen(libname)-1]=0;
			}

			lib=dlopen(libname, RTLD_LAZY);
			err=SECERR_UNKNOWNERROR;

			if ((!lib)||(strlen(libname)==0))
			{
				printf("cannot open library '%s' - '%s'\n",libname, dlerror());
			}
			else 

			{
				init_func ifunc=dlsym(lib,CFG_INITFUNC);

				if (ifunc==0)
				{
					printf("cannot load function '%s' - '%s'\n",CFG_INITFUNC, dlerror());
				}
				else
				{
					ifunc();
				}
				
				if (uname!=0)
				{
					byname=dlsym(lib,CFG_BYNAMEFUNC);

					if (byname==0)
					{
						printf("cannot load function '%s'",CFG_BYNAMEFUNC);
					}
					else
					{
						err=byname(uname, outs);
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
						err=byuid(uid, outs);
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

void priv_init()
{
	printf("debug: securepriv_look.c initialized\n");
	enum_db=enum_priority_list;
	printf("enum_db: %d\n", enum_db);
}
