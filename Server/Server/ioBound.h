//
//  ioBound.h
//  Server
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#ifndef Server_ioBound_h
#define Server_ioBound_h

typedef struct sOutbound Outbound, *pOutbound;
typedef struct sInbound Inbound, *pInbound;
typedef struct sInboundExtended InboundExtended, *pInboundExtended;

struct sOutbound
{
    unsigned long len;
    int pid;
    int userLevel;
    char username[20];
    char pw[20];
    int jobRef;
    int nArgs;
    char *args;
};

struct sInbound
{
    int valid;
};

struct sInboundExtended
{
    int valid;
    unsigned long len;
    char *report;
};

#endif
