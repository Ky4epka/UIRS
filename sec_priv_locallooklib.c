#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sec_priv_locallooklib.h"
#include <sys/capability.h>

#define STRUCT_ERR_FLAG_EMPTY_USER_NAME 	0x0001
#define STRUCT_ERR_FLAG_NEGATIVE_USER_ID 	0x0002
#define STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS 	0x0004
#define STRUCT_ERR_FLAG_EMPTY_CAPS 		0x0008
#define STRUCT_ERR_FLAG_BAD_CAPS 		0x0010

#define STRUCT_ERR_FLAG_EMPTY_USER_NAME_STR 	"EMPTY_USER_NAME"
#define STRUCT_ERR_FLAG_NEGATIVE_USER_ID_STR 	"NEGATIVE_USER_ID"
#define STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS_STR 	"WRONG_LEVEL_BOUNDS"
#define STRUCT_ERR_FLAG_EMPTY_CAPS_STR 		"EMPTY_SECURITY_CAPS"
#define STRUCT_ERR_FLAG_BAD_CAPS_STR 		"BAD_SECURITY_CAPS"

void *last_struct=NULL;
char struct_err_string[2048];

void print_struct(const struct sec_priv_usersec_struct *outs)
{
	_DEBUGLOG("	%s", outs->user_name);
	_DEBUGLOG("	%d", outs->uid);
	_DEBUGLOG("	%d", outs->security_level_min);
	_DEBUGLOG("	%d", outs->security_level_max);
	
	if (outs->security_caps!=NULL)
	{
		_DEBUGLOG("	%s", outs->security_caps);
	}

}


uint64_t str_to_uint64(const char *str)
{
	uint64_t num=0;
	bool numstarted=false;

	int len=strlen(str);
	int i=len-1;
	uint64_t mul=1;

	while (i>=0)
	{

		if ((str[i]>='0')&&(str[i]<='9'))
		{
			numstarted=true;
			num+=mul*(str[i]-'0');
			mul=mul*10;
		}
		else if (numstarted)
		{
			break;
		}

		i--;
	}

	return (num);
}

uint32_t check_struct(const struct sec_priv_usersec_struct *outs)
{
	uint32_t err_flags=0;

	if ((outs->user_name==NULL)||(strlen(outs->user_name)<=1))
	{
		err_flags=err_flags|STRUCT_ERR_FLAG_EMPTY_USER_NAME;
	}

	if (outs->uid<0)
	{
		err_flags=err_flags|STRUCT_ERR_FLAG_NEGATIVE_USER_ID;
	}

	if (outs->security_level_min>outs->security_level_max)
	{
		err_flags=err_flags|STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS;
	}

	if ((outs->security_caps==NULL)||(strlen(outs->security_caps)<=1))
	{
		err_flags=err_flags|STRUCT_ERR_FLAG_EMPTY_CAPS;
	}
	
	cap_t check_caps=cap_from_text(outs->security_caps);
	if (check_caps==NULL)
	{
		err_flags=err_flags|STRUCT_ERR_FLAG_BAD_CAPS;
	}
	else
	{
		cap_free(check_caps);
	}

	return (err_flags);
}

const char *check_flags_to_string(uint32_t check_flags)
{
	bool prev_cat=false;
	struct_err_string[0]=0;

	if ((check_flags&STRUCT_ERR_FLAG_EMPTY_USER_NAME)==STRUCT_ERR_FLAG_EMPTY_USER_NAME)
	{
		strcpy(struct_err_string, STRUCT_ERR_FLAG_EMPTY_USER_NAME_STR);
		prev_cat=true;
	}

	if ((check_flags&STRUCT_ERR_FLAG_NEGATIVE_USER_ID)==STRUCT_ERR_FLAG_NEGATIVE_USER_ID)
	{

		if (prev_cat)
		{
			strcat(struct_err_string, " | ");
			strcat(struct_err_string, STRUCT_ERR_FLAG_NEGATIVE_USER_ID_STR);
		}
		else
		{
			strcpy(struct_err_string, STRUCT_ERR_FLAG_NEGATIVE_USER_ID_STR);
		}

		prev_cat=true;
	}

	if ((check_flags&STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS)==STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS)
	{

		if (prev_cat)
		{
			strcat(struct_err_string, " | ");
			strcat(struct_err_string, STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS_STR);
		} 
		else
		{
			strcpy(struct_err_string, STRUCT_ERR_FLAG_WRONG_LEVEL_BOUNDS_STR);
		}

		prev_cat=true;
	}

	if ((check_flags&STRUCT_ERR_FLAG_EMPTY_CAPS)==STRUCT_ERR_FLAG_EMPTY_CAPS)
	{


		if (prev_cat)
		{
			strcat(struct_err_string, " | ");
			strcat(struct_err_string, STRUCT_ERR_FLAG_EMPTY_CAPS_STR);
		} 
		else
		{
			strcpy(struct_err_string, STRUCT_ERR_FLAG_EMPTY_CAPS_STR);
		}

		prev_cat=true;
	}

	if ((check_flags&STRUCT_ERR_FLAG_BAD_CAPS)==STRUCT_ERR_FLAG_BAD_CAPS)
	{


		if (prev_cat)
		{
			strcat(struct_err_string, " | ");
			strcat(struct_err_string, STRUCT_ERR_FLAG_BAD_CAPS_STR);
		} 
		else
		{
			strcpy(struct_err_string, STRUCT_ERR_FLAG_BAD_CAPS_STR);
		}

		prev_cat=true;
	}

	return (struct_err_string);
}

void str_to_struct(const char *str, struct sec_priv_usersec_struct *outs, bool *out_of_mem)
{
	int len=strlen(str);
	char *word=NULL;
	*out_of_mem=false;

	if (strlen(str)==0)
	{
		return;
	}

	char *buf=malloc(len+1);

	if (buf==NULL)
	{
		*out_of_mem=true;
		return;
	}

	int counter=0;

	strcpy(buf, str);
	word=strtok(buf, LOCAL_DB_LINE_DELIMITERS);
	while (word!=NULL)
	{

		if (counter==0)
		{
			outs->user_name=malloc(strlen(word));

			if (outs->user_name==NULL)
			{
				*out_of_mem=true;
				return;
			}

			strcpy(outs->user_name, word);
		}
		else if (counter==1)
		{
			outs->uid=atoi(word);
		}
		else if (counter==2)
		{
			outs->security_level_min=atoi(word);
		}
		else if (counter==3)
		{
			outs->security_level_max=atoi(word);
		}

		counter++;
		word=strtok(NULL, LOCAL_DB_LINE_DELIMITERS);
	}

	strcpy(buf, str);
	word=strtok(buf, LOCAL_DB_CAPS_BOUND_DELIMITERS);
	word=strtok(NULL, LOCAL_DB_CAPS_BOUND_DELIMITERS);
	outs->security_caps=NULL;

	if (word!=NULL)
	{
		outs->security_caps=malloc(strlen(word)+1);

		if (outs->security_caps==NULL)
		{
			*out_of_mem=true;
			return;
		}

		strcpy(outs->security_caps, word);
	}

	free(buf);
}

bool enum_local_db(const char *uname, const uid_t uid, struct sec_priv_usersec_struct **outs)
{
	*outs=NULL;

	if ((uname==NULL)&&(uid==0))
	{
		sec_priv_set_last_error(SECERR_UNKNOWN_ERROR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	FILE *f=fopen(LOCAL_DBFILE, "r+t");

	if (f==NULL)
	{
		sec_priv_set_last_error(SECERR_DB_ERROR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	char *line=malloc(LOCAL_DB_LINE_MAX_SIZE);
	struct sec_priv_usersec_struct *buf=malloc(sizeof(struct sec_priv_usersec_struct));

	if ((buf==NULL)||(line==NULL))
	{
		fclose(f);
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}
	
	bool found=false;
	bool out_of_mem=false;
	uint32_t check_flags=0;
	uint64_t line_count=0;
	
	while (!feof(f))
	{
		line_count++;

		if (fgets(line, LOCAL_DB_LINE_MAX_SIZE, f)==NULL)
		{
			break;
		}

		str_to_struct(line, buf, &out_of_mem);

		if (out_of_mem)
		{
			sec_priv_set_last_error(SECERR_OUT_OF_MEM);
			_ERRLOG("has returned '%s'", sec_priv_get_last_error());
			return (false);
		}

		check_flags=check_struct(buf);

		_DEBUGLOG("Struct", "");
		print_struct(buf);

		if (check_flags!=0)
		{
			_ERRLOG("Database line '%d' error: '%s'", line_count, check_flags_to_string(check_flags));
			
			if (buf->user_name!=NULL)
			{
				free(buf->user_name);
			}

			continue;
		}
		

		if (uname!=NULL)
		{
			found=(strcmp(uname, buf->user_name)==0);
		}
		else
		{
			found=(uid==buf->uid);
		}
		
		if (found)
		{
			break;
		}
		else if (buf->user_name!=NULL)
		{
			free(buf->user_name);
		}

	}

	free(line);
	fclose(f);

	if (found)
	{
		*outs=buf;
		sec_priv_set_last_error(SECERR_SUCCESS);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (true);
	}
	else
	{
		free(buf);
	}

	if (uname!=NULL)
	{
		sec_priv_set_last_error(SECERR_USER_NAME_NOT_FOUND);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}
	else
	{
		sec_priv_set_last_error(SECERR_USER_ID_NOT_FOUND);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error());
		return (false);
	}

	sec_priv_set_last_error(SECERR_UNKNOWN_ERROR);
	_ERRLOG("has returned '%s'", sec_priv_get_last_error());
	return (false);
}

bool sec_priv_get_by_uname(struct sec_priv_usersec_struct **outs, const void *params)
{
	last_struct=params;
	struct sec_priv_user_info_params_struct *s=params;

	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (s==NULL)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_EMPTY_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (strcmp(s->signature, COMMON_STRUCT_SIGNATURE)!=0)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_INVALID_SIGNATURE);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	if ((s->user_name==NULL)||(strlen(s->user_name)==0))
	{
		sec_priv_set_last_error(SECERR_EMPTY_USER_NAME);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	return (enum_local_db(s->user_name, 0, outs));
}

bool sec_priv_get_by_uid(struct sec_priv_usersec_struct **outs, const void *params)
{
	last_struct=params;
	struct sec_priv_user_info_params_struct *s=params;

	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (s==NULL)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_EMPTY_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (strcmp(s->signature, COMMON_STRUCT_SIGNATURE)!=0)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_INVALID_SIGNATURE);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	if (s->user_id==0)
	{
		sec_priv_set_last_error(SECERR_EMPTY_USER_ID);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	return (enum_local_db(NULL, s->user_id, outs));
}

void sec_priv_init()
{
	sec_priv_dl_init();
	_DEBUGLOG("'sec_priv_locallooklib.so' initialized", "")
}

void sec_priv_free_params(void *s)
{
	struct sec_priv_user_info_params_struct *st=s;

	if (st!=NULL)
	{

		if (strcmp(st->signature, COMMON_STRUCT_SIGNATURE)!=0)
		{

			if (st->user_name!=NULL)
			{
				free(st->user_name);
			}

			free(st);
		}
	}

}
