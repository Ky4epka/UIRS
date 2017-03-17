#ifndef __SECLOCALLOOKLIB_6f0a281e09faf7e920d6f287e2d23e768b7412aa
#define __SECLOCALLOOKLIB_6f0a281e09faf7e920d6f287e2d23e768b7412aa


#define LOCAL_DBFILE			"local.db"
#define LOCAL_DB_LINE_MAX_SIZE 		1024
#define LOCAL_DB_LINE_DELIMITERS 	" |"
#define LOCAL_DB_STRUCT_SIGNATURE	"LDB_STRUCT"
#define LOCAL_DB_STRUCT_SIGNATURE_LEN	10


#define SECERR_USER_NAME_NOT_FOUND	"SECERR_USER_NAME_NOT_FOUND"
#define SECERR_USER_ID_NOT_FOUND	"SECERR_USER_ID_NOT_FOUND"
#define SECERR_EMPTY_USER_NAME		"SECERR_EMPTY_USER_NAME"
#define SECERR_EMPTY_USER_ID		"SECERR_EMPTY_USER_ID"

struct sec_priv_local_db_params_struct
{
	char signature[LOCAL_DB_STRUCT_SIGNATURE_LEN+1];
	char *user_name;
	uid_t user_id;
};


extern bool sec_priv_get_uname(struct sec_priv_usersec_struct **outs, const void *params);
extern bool sec_priv_get_uid(struct sec_priv_usersec_struct **outs, const void *params);

#endif
