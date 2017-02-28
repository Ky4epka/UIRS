#include <stdlib.h>
#include <string.h>
#include "securetypes.h"

extern sec_priv_enum_db_func enum_db;
extern FILE* bio_stream;
extern char last_error[];

sec_priv_enum_db_func enum_db=0;
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
	buf->security_level=src->security_level;
	buf->security_category=src->security_category;
}

bool sec_priv_get_name(const char *uname, struct sec_priv_usersec_struct **outs)
{

	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned 'SECERR_NULL_STRUCT_PTR'", "")
		return (false);
	}
	else if (uname==NULL)
	{
		sec_priv_set_last_error(SECERR_EMPTY_USER_NAME);
		_ERRLOG("has returned 'SECERR_EMPTY_USER_NAME'", "")
		return (false);
	}

	if (enum_db==NULL)
	{
		sec_priv_set_last_error(SECERR_NOT_INITIALIZED);
		_ERRLOG("has returned 'SECERR_NOT_INITIALIZED'", "")
		return (false);
	}

	return (sec_priv_get_enum_db_func()(uname, 0, outs));
}

bool sec_priv_get_uid(const uid_t uid, struct sec_priv_usersec_struct **outs)
{
	
	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned 'SECERR_NULL_STRUCT_PTR'", "")
		return (false);
	}
	else if (uid==0)
	{
		sec_priv_set_last_error(SECERR_EMPTY_UID);
		_ERRLOG("has returned 'SECERR_EMPTY_UID'", "")
		return (false);
	}
	
	if (enum_db==NULL)
	{
		sec_priv_set_last_error(SECERR_NOT_INITIALIZED);
		_ERRLOG("has returned 'SECERR_NOT_INITIALIZED'", "")
		return (false);
	}

	return (sec_priv_get_enum_db_func()(NULL, uid, outs));
}

void sec_priv_dl_init()
{
	sec_priv_set_error_log_stream(NULL);
}

const char * sec_priv_get_last_error()
{
	return (last_error);
}

void sec_priv_set_enum_db_func(const sec_priv_enum_db_func func)
{
	enum_db=func;
}

sec_priv_enum_db_func sec_priv_get_enum_db_func()
{
	return (enum_db);
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
