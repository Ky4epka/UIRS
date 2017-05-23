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
	sec_priv_init();
	rv=pam_get_user(pamh, &user, NULL);

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

	sprintf(level_prompt, "Enter privileges level: ");
	msg.msg_style=PAM_PROMPT_ECHO_ON;
	msg.msg=level_prompt;
	msgp[0]=&msg;
	rv=pam_get_item(pamh, PAM_CONV, (const void **)&conv);

	if (rv!=PAM_SUCCESS)
	{
		rv=PAM_AUTHINFO_UNAVAIL;
		goto out;
	}

	if ((conv==NULL)||(conv->conv==NULL))
	{
		rv=PAM_AUTHINFO_UNAVAIL;
		goto out;
	}

	rv=conv->conv(1,(const struct pam_message **)msgp, &resp, conv->appdata_ptr);

	if (rv!=PAM_SUCCESS)
	{
		rv=PAM_AUTHINFO_UNAVAIL;
		goto out;
	}
	
	if ((resp==NULL)||(resp[0].resp==NULL))
	{
		rv=PAM_AUTHINFO_UNAVAIL;
		goto out;
	}

	level=strdup(resp[0].resp);
	memset(resp[0].resp, 0, strlen(resp[0].resp));
	free(&resp[0]);
	int ilvl=-1;

	if (level!=NULL)
	{
		ilvl=atoi(level);
	}

	if ((ilvl<outs->security_level_min)||(ilvl>outs->security_level_max))
	{
		pam_syslog(pamh, LOG_ERR, "This user '%s' not have acces for this security level '%s'.", user, level);
		rv=PAM_AUTHINFO_UNAVAIL;
		goto out;
	}

	cap_t caps=cap_from_text(outs->security_caps);
	capsetp(getpid(), caps);
	rv=PAM_SUCCESS;
	goto out;
out:
	sec_priv_free();
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
	pam_sm_authenticate(0,0,0,0);
}
