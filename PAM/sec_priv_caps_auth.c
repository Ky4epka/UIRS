//#define PAM_SM_AUTH
//#define PAM_SM_ACCOUNT
//#define PAM_SM_SESSION
//#define PAM_SM_PASSWORD
#include <sys/types.h>
#include <sys/syslog.h>
#include <sys/capability.h>
#include <unistd.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include "sec_priv_caps_auth.h"
#include "sec_priv_interface_share.h"
#include "sec_priv_interface_share_funcs.h"

struct _cap_struct {
    struct __user_cap_header_struct head;
    struct __user_cap_data_struct set;
};

	int i, rv;
	char *user;
	char *level;
	char level_prompt[64];
	struct sec_priv_usersec_struct *outs=NULL;
	struct pam_conv *conv; 
	struct pam_message msg;
	struct pam_message *(msgp[1]);
	struct pam_response *resp;


PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	outs=0;
	sec_priv_init();
	rv=pam_get_user(pamh, &user, NULL);

	printf("founding user...\n");

	if (rv!=PAM_SUCCESS)
	{
		pam_syslog(pamh, LOG_ERR, "pam_get_user() failed %s", pam_strerror(pamh, rv));
		rv=PAM_USER_UNKNOWN;
		goto out;
	}
	else if (!sec_priv_get_by_uname(user, &outs))
	{
		pam_syslog(pamh, LOG_ERR, "User not found in privileges database %s", user);
		rv=PAM_USER_UNKNOWN;
		goto out;
	}

	printf("user founded: %s\n", user);
	printf("user founded: %s\n", user);
	printf("caps...: %s\n", outs->security_caps);
	cap_t caps=cap_from_text(outs->security_caps);
	printf("parsing caps...: %s\n", outs->security_caps);
	capsetp(getpid(), caps);
	printf("caps set\n");
	rv=PAM_SUCCESS;
	goto out;
out:
	sec_priv_free(outs);
	return rv;
}


int pam_setcred(pam_handle_t *pamh, int flags)
{
	return PAM_SUCCESS ;
}


int pam_acct_mgmt(pam_handle_t *pamh, int flags)
{
	return PAM_SUCCESS ;
}


int pam_open_session(pam_handle_t *pamh, int flags)
{
	return PAM_SUCCESS ;
}

int pam_close_session(pam_handle_t *pamh, int flags)
{
	return PAM_SUCCESS ;
}

extern void test_func()
{
	struct sec_priv_usersec_struct *outs=0;
	printf("sec_priv_init();\n");
	sec_priv_init();
	printf("sec_priv_get_by_uname();\n");
	sec_priv_get_by_uname("user1", &outs);
	printf("sec_priv_free();\n");
	sec_priv_free(outs);
	printf("end\n");
	//pam_sm_authenticate(0,0,0,0);
}
