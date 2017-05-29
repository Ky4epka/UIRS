#include <stdlib.h>
#include <string.h>
#include "sec_priv_types.h"

extern FILE* bio_stream;
extern char last_error[];

FILE* bio_stream=NULL;
char last_error[ERROR_STRING_MAX_LENGTH];



bool sec_priv_copy_struct(const struct sec_priv_usersec_struct *src, struct sec_priv_usersec_struct **dst)
{
	
	if ((src==NULL)||(dst==NULL))
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned 'SECERR_NULL_STRUCT_PTR'", "")
		return (false);
	}

	*dst=malloc(sizeof(struct sec_priv_usersec_struct));

	if (*dst==NULL)
	{
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		_ERRLOG("has returned 'SECERR_OUT_OF_MEM'", "")
		return (false);
	}

	struct sec_priv_usersec_struct *buf=*dst;

	buf->user_name=malloc(strlen(src->user_name));

	if (buf->user_name==NULL)
	{
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		_ERRLOG("has returned 'SECERR_OUT_OF_MEM'", "")
		return (false);
	}

	strcpy(buf->user_name, src->user_name);
	buf->uid=src->uid;
	buf->security_caps=malloc(strlen(src->security_caps));
	strcpy(buf->security_caps, src->security_caps);
	return (true);
}


void sec_priv_dl_init()
{
	sec_priv_set_error_log_stream(NULL);
}

const char * sec_priv_get_last_error()
{
	return (last_error);
}

void sec_priv_set_error_log_stream(const FILE *bios)
{

	if (bios==NULL)
	{
		bio_stream=stderr;
	}
	else
	{
		bio_stream=bios;
	}

}

FILE *sec_priv_get_error_log_stream()
{
	return (bio_stream);
}

void sec_priv_set_last_error(const char *error)
{
	strcpy(last_error, error);
}
