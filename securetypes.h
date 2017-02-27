
#define SECERR_SUCCESS			0
#define SECERR_NULLSTRUCTPTR		1
#define SECERR_OUTOFMEM			2
#define SECERR_EMPTYUSERNAME		3
#define SECERR_EMPTYUID			4
#define SECERR_CFGREADERROR		5
#define SECERR_NAMENOTFOUND		6
#define SECERR_UIDNOTFOUND		7
#define SECERR_DBERROR			8
#define SECERR_NOTINITIALIZED   	9
#define SECERR_UNKNOWNERROR		999


struct usersec_struct
{
	char *user_name;
	long uid;

	int security_level;
	char security_category[11];
};

#define CFG_BYNAMEFUNC	"getsecprivname"
#define CFG_BYUIDFUNC	"getsecprivuid"
#define CFG_INITFUNC 	"priv_init"

typedef int (*enum_db_func) (char *, int, struct usersec_struct **);
typedef int (*getsecprivname_func) (char *, struct usersec_struct **);
typedef int (*getsecprivuid_func) (int, struct usersec_struct **);
typedef void (*init_func) ();

extern int getsecprivname(char *uname, struct usersec_struct **outs);
extern int getsecprivid(int uid, struct usersec_struct **outs);
extern void priv_init();

extern enum_db_func enum_db;
void copystruct(struct usersec_struct *src, struct usersec_struct *dst);
