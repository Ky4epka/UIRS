#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <sys/capability.h>
#include "sec_priv_interface_share.h"
#include "sec_priv_interface_share_funcs.h"
#include "sec_priv_caps_auth.h"

struct _cap_struct {
    struct __user_cap_header_struct head;
    struct __user_cap_data_struct set;
};

int main(int argc, char *argv[], char *envp[])
{
	cap_t cap=cap_get_proc();
	ssize_t len;
	char *str=strdup(cap_to_text(cap, &len));
	printf("PID: %d\n", cap->head.pid);
	printf("CAPS: %d %s\n", len, str);
	printf("EFF: %d\n", cap->set.effective);
	free(str);
	str=strdup("all=pe cap_chown-e cap_kill-ep");
	cap_t caps=cap_from_text(str);

	if (caps!=NULL)
	{
		printf("caps successfull %d\n", caps);
	}
	else
	{
		printf("caps is empty\n");
	}

	test_func();
	return (0);
}

