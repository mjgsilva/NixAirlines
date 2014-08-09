/*
 ioBound.h
 Terminal
 
 Created by Mário Jorge Gabriel da Silva on 15/06/13.
 Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
 */

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

