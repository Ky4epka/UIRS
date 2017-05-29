#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "sec_priv_interface_share.h"
#include "sec_priv_interface_share_funcs.h"
#include <sys/capability.h>

struct _cap_struct {
    struct __user_cap_header_struct head;
    struct __user_cap_data_struct set;
};

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

	struct sec_priv_usersec_struct *outs=NULL;
	sec_priv_init();

	if ((uname==NULL)||(strlen(uname)==0))
	{
		err_result=sec_priv_get_by_uid(uid, &outs);
	}
	else
	{
		err_result=sec_priv_get_by_uname(uname, &outs);
	}

	sec_priv_free(outs);
	_DEBUGLOG("end", "");

	cap_t cap=cap_get_proc();
	ssize_t len;
	char *str=strdup(cap_to_text(cap, &len));
	printf("PID: %d", cap->head.pid);
	printf("CAPS: %d %s\n", len, str);
	printf("EFF: %d", cap->set.effective);
	free(str);
	return (0);
}
