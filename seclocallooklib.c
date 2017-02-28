#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "securetypes.h"
#include "seclocallooklib.h"

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

bool enum_localdb(const char *uname, const uid_t uid, struct sec_priv_usersec_struct **outs)
{
	*outs=NULL;

	if ((uname==NULL)&&(uid==0))
	{
		sec_priv_set_last_error(SECERR_UNKNOWN_ERROR);
		_ERRLOG("has returned 'SECERR_UNKNOWN_ERROR'", "")
		return (false);
	}

	FILE *f=fopen(LOCAL_DBFILE, "r+t");

	if (f==NULL)
	{
		sec_priv_set_last_error(SECERR_DB_ERROR);
		_ERRLOG("has returned 'SECERR_DB_ERROR'", "")
		return (false);
	}

	char *line=malloc(LOCAL_DB_LINE_MAX_SIZE);
	struct sec_priv_usersec_struct *buf=malloc(sizeof(struct sec_priv_usersec_struct));

	if ((buf==NULL)||(line==NULL))
	{
		fclose(f);
		sec_priv_set_last_error(SECERR_OUT_OF_MEM);
		_ERRLOG("has returned 'SECERR_OUT_OF_MEM'", "")
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
		_ERRLOG("has returned 'SECERR_SUCCESS'", "")
		return (true);
	}
	else
	{
		free(buf);
	}

	if (uname!=NULL)
	{
		sec_priv_set_last_error(SECERR_NAME_NOT_FOUND);
		_ERRLOG("has returned 'SECERR_NAME_NOT_FOUND'", "")
		return (false);
	}
	else
	{
		sec_priv_set_last_error(SECERR_UID_NOT_FOUND);
		_ERRLOG("has returned 'SECERR_UID_NOT_FOUND'", "")
		return (false);
	}

	sec_priv_set_last_error(SECERR_UNKNOWN_ERROR);
		_ERRLOG("has returned 'SECERR_UNKNOWN_ERROR'", "")
	return (false);
}

void sec_priv_init()
{
	sec_priv_dl_init();
	sec_priv_set_enum_db_func(enum_localdb);
	_DEBUGLOG("'seclocallooklib.so' initialized", "")
}
