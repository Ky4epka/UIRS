#define SECERR_SUCCESS			0
#define SECERR_NULLSTRUCTPTR	1
#define SECERR_OUTOFMEM			2
#define SECERR_EMPTYUSERNAME	3
#define SECERR_EMPTYUID			4
#define SECERR_CFGREADERROR		5
#define SECERR_NAMENOTFOUND		6
#define SECERR_UIDNOTFOUND		7
#define SECERR_DBERROR			8
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

extern int getsecprivname(char *uname, usersec_struct **outs);
extern int getsecprivid(int uid, usersec_struct **outs);

void copystruct(usersec_struct *src, usersec_struct *dst);