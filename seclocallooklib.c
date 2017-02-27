#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "securetypes.h"
#include "seclocallooklib.h"

void strtostruct(char *str, struct usersec_struct *outs)
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

int enum_localdb(char *uname, int uid, struct usersec_struct **outs)
{
	*outs=0;

	if ((uname==0)&&(uid==0))
	{
		return (SECERR_UNKNOWNERROR);
	}

	FILE *f=fopen(LOCAL_DBFILE, "r+t");

	if (f!=0)
	{
		char *line=malloc(LOCAL_DBLINEMAXSIZE);
		struct usersec_struct *buf=malloc(sizeof(struct usersec_struct));

		if ((buf==0)||(line==0))
		{
			fclose(f);
			return (SECERR_OUTOFMEM);
		}
		
		int found=0;
		
		while (!feof(f))
		{
			fgets(line, LOCAL_DBLINEMAXSIZE, f);
			strtostruct(line, buf);

			if (uname!=0)
			{
				
				if (strcmp(uname, buf->user_name)==0)
				{
					found=1;
				}

			}
			else
			{

				if (uid==buf->uid)
				{
					found=1;
				}

			}
			
			if (found==1)
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

		if (found==1)
		{
			*outs=buf;
			return (SECERR_SUCCESS);
		}
		else
		{
			free(buf);
		}

	}
	else
	{
		return (SECERR_DBERROR);
	}

	if (uname!=0)
	{
		return (SECERR_NAMENOTFOUND);
	}
	else
	{
		return (SECERR_UIDNOTFOUND);
	}

	return (SECERR_UNKNOWNERROR);
}

void priv_init()
{
	printf("debug: seclocallooklib.so initialized\n");
	enum_db=enum_localdb;
	printf("enum_db: %d\n", enum_db);
}
