#include <stdlib.h>
#include <string.h>
#include "securetypes.h"

enum_db_func enum_db=0;

void copystruct(struct usersec_struct *src, struct usersec_struct *dst)
{
	
	if ((src==0)||(dst==0))
	{
		return;
	}

	dst->user_name=malloc(strlen(src->user_name));
	strcpy(dst->user_name, src->user_name);
	dst->uid=src->uid;
	dst->security_level=src->security_level;
	strcpy(dst->security_category,src->security_category);
}

int getsecprivname(char *uname, struct usersec_struct **outs)
{

	if (outs==0)
	{
		return (SECERR_NULLSTRUCTPTR);
	}
	else if (uname==0)
	{
		return (SECERR_EMPTYUSERNAME);
	}

	if (enum_db==0)
	{
		return (SECERR_NOTINITIALIZED);
	}

	return (enum_db(uname, 0, outs));
}

int getsecprivid(int uid, struct usersec_struct **outs)
{
	
	if (outs==0)
	{
		return (SECERR_NULLSTRUCTPTR);
	}
	else if (uid==0)
	{
		return (SECERR_EMPTYUID);
	}
	
	if (enum_db==0)
	{
		return (SECERR_NOTINITIALIZED);
	}

	return (enum_db(0, uid, outs));
}
