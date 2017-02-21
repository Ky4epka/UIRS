#include <stdlib.h>
#include <string.h>
#include "securetypes.h"


void copystruct(usersec_struct *src, usersec_struct *dst)
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