#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include "sec_priv_interface_share.h"

int i, rv;
const char *user; // имя пользователя
char *password; // вводимый пользователем пароль
char password_prompt[64]; // запрос на ввод пароля, показываемый приложением
// структуры PAM
struct pam_conv *conv; // функция диалога PAM
struct pam_message msg; // сообщения диалога PAM
struct pam_message *(msgp[1]);
struct pam_response *resp; // ответ PAM


PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	rv=pam_get_user(pamh, &user, NULL);

	if (rv!=PAM_SUCCESS)
	{
		pam_syslog(pamh, LOG_ERR, "pam_get_user() failed %s", pam_strerror(pamh, rv));
		return (PAM_USER_UNKNOWN);
	}

	rv=pam_get_item(pamh, PAM_AUTHTOK, (void *)&password);

	if (rv==PAM_SUCCESS && password) 
	{
		password=strdup(password);
	}
	else
	{
		sprintf(password_prompt, "Password for auth: ");
		msg.msg_style=PAM_PROMPT_ECHO_OFF;
		msg.msg=password_prompt;
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

		password=strdup(resp[0].resp);
		memset(resp[0].resp, 0, strlen(resp[0].resp));
		free(&resp[0]);
	}

	return PAM_SUCCESS ;

out:
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
