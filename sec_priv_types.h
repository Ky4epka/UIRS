#ifndef __SECURETYPES_9dce1ff27c56c492b6c20025461c5147be6121ec
#define __SECURETYPES_9dce1ff27c56c492b6c20025461c5147be6121ec


#define ERROR_STRING_MAX_LENGTH			512

#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sec_priv_interface_share.h"

void sec_priv_dl_init();// Must be execute in sec_priv_init func

extern void sec_priv_init();// Must be defined in module lib

extern const char *sec_priv_get_last_error();
extern void sec_priv_set_error_log_stream(const FILE* bios);
extern bool sec_priv_copy_struct(const struct sec_priv_usersec_struct *src, struct sec_priv_usersec_struct **dst);

FILE* sec_priv_get_error_log_stream();
void sec_priv_set_last_error(const char *error);

#endif
