#ifndef __SECUREPRIV_LOOK_008c4105090249c17e32e8407db2195bd9d60fbf
#define __SECUREPRIV_LOOK_008c4105090249c17e32e8407db2195bd9d60fbf

#define CFG_FILE				"settings.cfg"

/*
	CFG Format

	--- Priority library list
	libnamefile1
	libnamefile2
	...
*/


extern bool sec_priv_get(const char *func_name, struct sec_priv_usersec_struct **outs, const void *params);

#endif
