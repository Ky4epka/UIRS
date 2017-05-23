#ifndef __SEC_PRIV_INTERFACE_SHARE_2edd41e175196dad0e4681aa543a483dc7d75a99
#define __SEC_PRIV_INTERFACE_SHARE_2edd41e175196dad0e4681aa543a483dc7d75a99

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

#define COMMON_FUNC_BY_USER_NAME "sec_priv_get_by_uname"
#define COMMON_FUNC_BY_USER_ID "sec_priv_get_by_uid"

#define COMMON_STRUCT_SIGNATURE "CDB_COMMON"
#define COMMON_STRUCT_SIGNATURE_LEN 10

#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct sec_priv_usersec_struct
{
	char *user_name;
	uid_t uid;

	uint32_t security_level_min;
	uint32_t security_level_max;
	char *security_caps;
};

struct sec_priv_user_info_params_struct
{
	char signature[COMMON_STRUCT_SIGNATURE_LEN+1];
	char *user_name;
	uid_t user_id;
};

typedef void (*sec_priv_init_func) ();
typedef void (*sec_priv_free_func) (struct sec_priv_usersec_struct *s);
typedef void (*sec_priv_free_params_func) (void *);
typedef bool (*sec_priv_getX_func) (struct sec_priv_usersec_struct **outs, const void *params);
typedef bool (*sec_priv_get_func) (const char* func_name, struct sec_priv_usersec_struct **outs, const void *params);
typedef const char * (*sec_priv_get_last_error_func) ();


#define CFG_INIT_FUNC 		"sec_priv_init"
#define CFG_FREE_FUNC		"sec_priv_free"
#define CFG_FREE_PARAMS_FUNC	"sec_priv_free_params"
#define CFG_GET_FUNC		"sec_priv_get"
#define CFG_GET_LAST_ERROR_FUNC	"sec_priv_get_last_error"

#define _DEBUGLOG(FRMT_STR, ...) { printf("debug '%s::%s()': "#FRMT_STR"\n",__FILE__,__PRETTY_FUNCTION__, __VA_ARGS__); }
#define _ERRLOG(FRMT_STR, ...) { printf("debug '%s::%s()' error: "#FRMT_STR"\n",__FILE__,__PRETTY_FUNCTION__, __VA_ARGS__); }


#endif
