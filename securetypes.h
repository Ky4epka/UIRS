#ifndef __SECURETYPES_9dce1ff27c56c492b6c20025461c5147be6121ec
#define __SECURETYPES_9dce1ff27c56c492b6c20025461c5147be6121ec


#define SECERR_SUCCESS			"SECERR_SUCCESS"
#define SECERR_NULL_STRUCT_PTR		"SECERR_NULL_STRUCT_PTR"
#define SECERR_OUT_OF_MEM		"SECERR_OUT_OF_MEM"
#define SECERR_CFG_READ_ERROR		"SECERR_CFG_READ_ERROR"
#define SECERR_NOT_INITIALIZED   	"SECERR_NOT_INITIALIZED"
#define SECERR_DB_ERROR			"SECERR_DB_ERROR"
#define SECERR_NAME_NOT_FOUND		"SECERR_NAME_NOT_FOUND"
#define SECERR_UID_NOT_FOUND		"SECERR_UID_NOT_FOUND"
#define SECERR_EMPTY_USER_NAME		"SECERR_EMPTY_USER_NAME"
#define SECERR_EMPTY_UID		"SECERR_EMPTY_UID"
#define SECERR_UNKNOWN_ERROR		"SECERR_UNKNOWN_ERROR"

#define ERROR_STRING_MAX_LENGTH		512

#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

struct sec_priv_usersec_struct
{
	char *user_name;
	uid_t uid;

	int security_level;
	uint64_t security_category;
};


typedef bool (*sec_priv_get_name_func) (const char *uname, struct sec_priv_usersec_struct **);
typedef bool (*sec_priv_get_uid_func) (const uid_t uid, struct sec_priv_usersec_struct **);
typedef void (*sec_priv_init_func) ();
typedef bool (*sec_priv_enum_db_func) (const char *uname, const uid_t uid, struct sec_priv_usersec_struct **);
typedef const char * (*sec_priv_get_last_error_func) ();

extern void sec_priv_init();// Must be defined by module lib
void sec_priv_dl_init();// Must be execute in sec_priv_init func

void sec_priv_set_enum_db_func(const sec_priv_enum_db_func func);// Use this for setup enum db function in sec_priv_init function
sec_priv_enum_db_func sec_priv_get_enum_db_func();


extern bool sec_priv_get_name(const char *uname, struct sec_priv_usersec_struct **outs);
extern bool sec_priv_get_uid(const uid_t uid, struct sec_priv_usersec_struct **outs);
extern const char *sec_priv_get_last_error();
extern void sec_priv_set_error_log_stream(const FILE* bios);
extern bool sec_priv_copy_struct(const struct sec_priv_usersec_struct *src, struct sec_priv_usersec_struct **dst);



#define CFG_BY_NAME_FUNC	"sec_priv_get_name"
#define CFG_BY_UID_FUNC		"sec_priv_get_uid"
#define CFG_INIT_FUNC 		"sec_priv_init"
#define CFG_GET_LAST_ERROR_FUNC	"sec_priv_get_last_error"

#define _DEBUGLOG(FRMT_STR, ...) { printf("debug '%s::%s()': "#FRMT_STR"\n",__FILE__,__PRETTY_FUNCTION__, __VA_ARGS__); }
#define _ERRLOG(FRMT_STR, ...) { printf("debug '%s::%s()' error: "#FRMT_STR"\n",__FILE__,__PRETTY_FUNCTION__, __VA_ARGS__); }

FILE* sec_priv_get_error_log_stream();
void sec_priv_set_last_error(const char *error);

#endif
