//
//  communication.c
//  Server
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#include "communication.h"

int writePipe(char *npN, pInbound out)
{
    int outbox;
    outbox = open(npN, O_WRONLY);
    if((access(npN,R_OK)) == -1)
    {
        return 0;
    }
    write(outbox,out,sizeof(Inbound));
    close(outbox);
    return 1;
}

int writePipeExtended(char *npN, pInboundExtended out)
{
    int outbox, validation;
    unsigned long len;
    outbox = open(npN, O_WRONLY);
    if((access(npN,R_OK)) == -1)
    {
        return 0;
    }
    
    validation = out->valid;
    len = out->len;
    write(outbox,&validation,sizeof(int));
    write(outbox,&len,sizeof(unsigned long));
    write(outbox,out->report,sizeof(char)*len);
    close(outbox);
    return 1;
}