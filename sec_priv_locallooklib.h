#ifndef __SECLOCALLOOKLIB_6f0a281e09faf7e920d6f287e2d23e768b7412aa
#define __SECLOCALLOOKLIB_6f0a281e09faf7e920d6f287e2d23e768b7412aa

#include <inttypes.h>
#include <sys/types.h>
#include "sec_priv_types.h"
#include "sec_priv_local_look_share.h"
#include "sec_priv_interface_share.h"

extern bool sec_priv_get_by_uname(struct sec_priv_usersec_struct **outs, const void *params);
extern bool sec_priv_get_by_uid(struct sec_priv_usersec_struct **outs, const void *params);
extern void sec_priv_free_params(void *s);



#endif
