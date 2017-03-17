#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "securetypes.h"
#include "seclocallooklib.h"

struct sec_priv_local_db_params_struct *last_struct=NULL;

void print_struct(const struct sec_priv_usersec_struct *outs)
{
	_DEBUGLOG("	%s", outs->user_name);
	_DEBUGLOG("	%d", outs->uid);
	_DEBUGLOG("	%d", outs->security_level);
	_DEBUGLOG("	%d", outs->security_category);
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

bool str_to_struct(const char *str, struct sec_priv_usersec_struct *outs)
{
	int len=strlen(str);
	char *word=NULL;

	/*if (word==NULL)
	{
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		_ERRLOG("has returned 'SECERR_OUT_OF_MEM'", "")
		return (false);
	}
	else*/ if (strlen(str)==0)
	{
		return (false);
	}

	int counter=0;

	word=strtok(str, LOCAL_DB_LINE_DELIMITERS);
	while (word!=NULL)
	{

		if (counter==0)
		{
			outs->user_name=malloc(strlen(word));
			strcpy(outs->user_name, word);
		}
		else if (counter==1)
		{
			outs->uid=atoi(word);
		}
		else if (counter==2)
		{
			outs->security_level=atoi(word);
		}
		else if (counter==3)
		{
			outs->security_category=str_to_uint64(word);
		}

		counter++;
		word=strtok(NULL, LOCAL_DB_LINE_DELIMITERS);
	}

	free(word);
	return (true);
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
	
	while (!feof(f))
	{
		fgets(line, LOCAL_DB_LINE_MAX_SIZE, f);

		if (!str_to_struct(line, buf))
		{
			continue;
		}

		_DEBUGLOG("Struct", "");
		print_struct(buf);

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
		else
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

bool sec_priv_get_uname(struct sec_priv_usersec_struct **outs, const void *params)
{
	last_struct=params;

	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (last_struct==NULL)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_EMPTY_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (strcmp(last_struct->signature, LOCAL_DB_STRUCT_SIGNATURE)!=0)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_INVALID_SIGNATURE);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	if ((last_struct->user_name==NULL)||(strlen(last_struct->user_name)==0))
	{
		sec_priv_set_last_error(SECERR_EMPTY_USER_NAME);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

printf("HELLO\n");
	return (enum_local_db(last_struct->user_name, 0, outs));
}

bool sec_priv_get_uid(struct sec_priv_usersec_struct **outs, const void *params)
{
	last_struct=params;

	if (outs==NULL)
	{
		sec_priv_set_last_error(SECERR_NULL_STRUCT_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (last_struct==NULL)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_EMPTY_PTR);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
		return (false);
	}

	if (strcmp(last_struct->signature, LOCAL_DB_STRUCT_SIGNATURE)!=0)
	{
		sec_priv_set_last_error(SECERR_PARAM_STRUCT_INVALID_SIGNATURE);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	if (last_struct->user_id==0)
	{
		sec_priv_set_last_error(SECERR_EMPTY_USER_ID);
		_ERRLOG("has returned '%s'", sec_priv_get_last_error())
	}

	return (enum_local_db(NULL, last_struct->user_id, outs));
}

void sec_priv_init()
{
	sec_priv_dl_init();
	_DEBUGLOG("'seclocallooklib.so' initialized", "")
}

void sec_priv_free()
{

	if (last_struct!=NULL)
	{

		if (last_struct->user_name!=NULL)
		{
			free(last_struct->user_name);
		}

		free(last_struct);
		last_struct=NULL;
	}

}
