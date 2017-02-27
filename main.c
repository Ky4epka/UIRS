#include "securetypes.h"
#include "securepriv_look.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

int main()
{
printf("\nbegin...\n\n\n\n");
	char *libname=malloc(FILENAME_MAX);
	strcpy(libname, "securepriv_look.so");
	void *lib=0;
	getsecprivname_func byname=0;
	getsecprivuid_func byuid=0;
	int err=0;
	char *uname=malloc(50);
	strcpy(uname, "hello");
	int uid=0;
	struct userpriv_struct *outs=0;

	lib=dlopen(libname, RTLD_LAZY);
	err=SECERR_UNKNOWNERROR;

	if (!lib)
	{
		printf("cannot open library '%s' - '%s'\n",libname,dlerror());
	}
	else
	{
		printf("dlopened\n");

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
				printf("cannot load function '%s' - '%s'\n",CFG_BYNAMEFUNC, dlerror());
			}
			else
			{
				err=byname(uname, &outs);
			}

		}
		else
		{
			byuid=dlsym(lib,CFG_BYUIDFUNC);

			if (byuid==0)
			{
				printf("cannot load function '%s'\n",CFG_BYNAMEFUNC);
			}
			else
			{
				err=byuid(uid, &outs);
			}

		}

		printf("Find result: %d\n", err);
		dlclose(lib);
	}

	free(uname);
	free(libname);
printf("\nend.\n");
	return (0);
}
