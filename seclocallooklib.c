#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "securepriv_look.h"
#include "seclocallooklib.h"

void strtostruct(char *str, usersec_struct *outs)
{
	int len=strlen(str);
	char *word=malloc(len);
	int wordlen=0;
	int counter=0;

	char c=0;
	int i=0;
	while (i<len)
	{
		c=str[i];

		if ((c=='|')||(i==len-1))
		{
			word[wordlen]=0;

			if (counter==0)
			{
				outs->user_name=malloc(wordlen);
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
			else if (counter==2)
			{
				strncpy(outs->security_category, word, 10);
			}

			wordlen=0;
			counter++;
		}
		else
		{
			word[wordlen++]=c;
		}

		i++;
	}

	free(word);
}

int enum_localdb(char *uname, int uid, usersec_struct *outs)
{

	if ((uname==0)&&(uid==0))
	{
		return (SECERR_UNKNOWNERROR);
	}

	FILE *f=fopen(LOCAL_DBFILE, "r+t");

	if (f!=0)
	{
		char *line=malloc(LOCAL_DBLINEMAXSIZE);
		usersec_struct *buf=malloc(sizeof(usersec_struct));
		bool found=false;
		
		while (!feof(f))
		{
			fgets(line, LOCAL_DBLINEMAXSIZE, f);
			strtostruct(line, buf);

			if (uname!=0)
			{
				
				if (strcmp(uname, buf->user_name)==0)
				{
					found=true;
				}

			}
			else
			{

				if (uid==buf->uid)
				{
					found=true;
				}

			}

			free(buf->user_name);

			if (found)
			{
				copystruct(buf, outs);
				break;
			}

		}

		free(line);
		fclose(f);

		if (found)
		{
			return (SECERR_SUCCESS);
		}

	}
	else
	{
		return (SECERR_DBERROR);
	}

	return (SECERR_UNKNOWNERROR);
}

int getsecprivname(char *uname, usersec_struct **outs)
{

	if (outs==0)
	{
		return (SECERR_NULLSTRUCTPTR);
	}
	else if (uname==0)
	{
		return (SECERR_EMPTYUSERNAME);
	}

	*outs=malloc(sizeof(usersec_struct));

	if (outs==0)
	{
		return (SECERR_OUTOFMEM);
	}

	return (enum_localdb(uname, 0, *outs));
}

int getsecprivid(int uid, usersec_struct **outs)
{
	
	if (outs==0)
	{
		return (SECERR_NULLSTRUCTPTR);
	}
	else if (uid==0)
	{
		return (SECERR_EMPTYUID);
	}
	
	*outs=malloc(sizeof(usersec_struct));

	if (outs==0)
	{
		return (SECERR_OUTOFMEM);
	}

	return (enum_localdb(0, uid, *outs));
}