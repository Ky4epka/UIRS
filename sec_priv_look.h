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

#include "sec_priv_types.h"
#include "sec_priv_config_kit.h"
#include "sec_priv_interface_share.h"
#include "sec_priv_interface_share_funcs.h"

extern bool sec_priv_get(const char *func_name, struct sec_priv_usersec_struct **outs, const void *params);
extern bool sec_priv_get_by_uname(const char *user_name, struct sec_priv_usersec_struct **outs);
extern bool sec_priv_get_by_uid(const uid_t user_id, struct sec_priv_usersec_struct **outs);
extern void sec_priv_free(struct sec_priv_usersec_struct *s);


#endif
