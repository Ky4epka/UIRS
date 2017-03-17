#ifndef __SECURETYPES_9dce1ff27c56c492b6c20025461c5147be6121ec
#define __SECURETYPES_9dce1ff27c56c492b6c20025461c5147be6121ec


#define SECERR_SUCCESS				"SECERR_SUCCESS"
#define SECERR_NULL_STRUCT_PTR			"SECERR_NULL_STRUCT_PTR"
#define SECERR_OUT_OF_MEM			"SECERR_OUT_OF_MEM"
#define SECERR_CFG_READ_ERROR			"SECERR_CFG_READ_ERROR"
#define SECERR_NOT_INITIALIZED   		"SECERR_NOT_INITIALIZED"
#define SECERR_DB_ERROR				"SECERR_DB_ERROR"
#define SECERR_EMPTY_FUNC_NAME			"SECERR_EMPTY_FUNC_NAME"
#define SECERR_UNKNOWN_ERROR			"SECERR_UNKNOWN_ERROR"
#define SECERR_EXPORT_FUNC_NOT_FOUND		"SECERR_EXPORT_FUNC_NOT_FOUND"
#define SECERR_PARAM_STRUCT_EMPTY_PTR		"SECERR_PARAM_STRUCT_EMPTY_PTR"
#define SECERR_PARAM_STRUCT_INVALID_SIGNATURE	"SECERR_PARAM_STRUCT_INVALID_SIGNATURE"

#define ERROR_STRING_MAX_LENGTH			512

#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct sec_priv_usersec_struct
{
	char *user_name;
	uid_t uid;

	int security_level;
	uint64_t security_category;
};

typedef void (*sec_priv_init_func) ();
typedef void (*sec_priv_free_func) ();
typedef bool (*sec_priv_getX_func) (struct sec_priv_usersec_struct **outs, const void *params);
typedef bool (*sec_priv_get_func) (const char* func_name, struct sec_priv_usersec_struct **outs, const void *params);
typedef const char * (*sec_priv_get_last_error_func) ();

void sec_priv_dl_init();// Must be execute in sec_priv_init func

extern void sec_priv_init();// Must be defined in module lib
extern void sec_priv_free();// Must be defined in module lib
extern const char *sec_priv_get_last_error();
extern void sec_priv_set_error_log_stream(const FILE* bios);
extern bool sec_priv_copy_struct(const struct sec_priv_usersec_struct *src, struct sec_priv_usersec_struct **dst);



#define CFG_INIT_FUNC 		"sec_priv_init"
#define CFG_FREE_FUNC		"sec_priv_free"
#define CFG_GET_LAST_ERROR_FUNC	"sec_priv_get_last_error"

#define _DEBUGLOG(FRMT_STR, ...) { printf("debug '%s::%s()': "#FRMT_STR"\n",__FILE__,__PRETTY_FUNCTION__, __VA_ARGS__); }
#define _ERRLOG(FRMT_STR, ...) { printf("debug '%s::%s()' error: "#FRMT_STR"\n",__FILE__,__PRETTY_FUNCTION__, __VA_ARGS__); }

FILE* sec_priv_get_error_log_stream();
void sec_priv_set_last_error(const char *error);

#endif
