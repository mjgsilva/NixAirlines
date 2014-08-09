typedef struct lOutbound Outbound, *pOutbound;
typedef struct lInbound Inbound, *pInbound;

struct lOutbound
{
    unsigned long len;
    int pid;
    int userLevel;
    int jobRef;
    int nArgs;
    char *args;
};

struct lInbound
{
    int valid;
    unsigned long len;
    char *report;
};