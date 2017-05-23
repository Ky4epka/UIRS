#ifndef __SEC_PRIV_INTERFACE_SHARE_FUNCS_675b4ca901abfbdfc28deb3d5d42a2a17dd850cc
#define __SEC_PRIV_INTERFACE_SHARE_FUNCS_675b4ca901abfbdfc28deb3d5d42a2a17dd850cc

#include "sec_priv_interface_share.h"

bool sec_priv_get(const char *func_name, struct sec_priv_usersec_struct **outs, const void *params);
bool sec_priv_get_by_uname(const char *user_name, struct sec_priv_usersec_struct **outs);
bool sec_priv_get_by_uid(const uid_t user_id, struct sec_priv_usersec_struct **outs);

void sec_priv_init();// Must be defined in module lib
void sec_priv_free(struct sec_priv_usersec_struct *s);

const char *sec_priv_get_last_error();
void sec_priv_set_error_log_stream(const FILE* bios);
bool sec_priv_copy_struct(const struct sec_priv_usersec_struct *src, struct sec_priv_usersec_struct **dst);

#endif
