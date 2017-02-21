#include "securetypes.h"

#define CFG_FILE				"settings.cfg"

/*
	CFG Format

	--- Priority library list
	libnamefile1
	libnamefile2
	...
*/

typedef int (getsecprivname_func) (char *, usersec_struct **);
typedef int (getsecprivuid_func) (int, usersec_struct **);

extern int getsecprivname(char *uname, usersec_struct **outs);
extern int getsecprivid(int uid, usersec_struct **outs);